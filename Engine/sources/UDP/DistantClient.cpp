#include "UDP/DistantClient.h"
#include "UDP/Serialization/Serializer.h"
#include "UDP/Serialization/Deserializer.h"
#include "UDP/ClientUDP.h"
#include "UDP/Messages.h"

namespace Bousk
{
	namespace Network
	{
		namespace UDP
		{
			std::chrono::milliseconds DistantClient::sTimeout = BOUSKNET_DEFAULT_UDP_TIMEOUT;

			// Initialisation des membres de DistantClient
			DistantClient::DistantClient(ClientUDP& client, const Address& addr, uint64 clientid)
				: mClient(client)
				, mAddress(addr)
				, mClientId(clientid)
				, mConnectionStartTime(Utils::Now())
				, mLastKeepAlive(Utils::Now())
			{}

			// fonction "onConnectionSent()" et "onConnectionReceived()" sont appel� que lorsque la connexion est �tablie avec succ�s
			void DistantClient::onConnectionSent()
			{
				if (mState == State::None)
				{
					mState = State::ConnectionSent;
					maintainConnection();
				}
				else if (mState == State::ConnectionReceived)
				{
					// We sent a connection and had received one : we do connect !
					onConnected();
				}
			}

			void DistantClient::onConnectionReceived()
			{
				if (mState == State::None)
				{
					mState = State::ConnectionReceived;
					maintainConnection();
					// Push incoming connection request to client
					mClient.onMessageReady(std::make_unique<Messages::IncomingConnection>(mAddress, mClientId));
				}
				else if (mState == State::ConnectionSent)
				{
					// We received a connection and had sent one : we do connect !
					onConnected();
				}
			}

			// permet de g�rer l'�tat de la connexion en cas de coupure/soucis du r�seau
		#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			void DistantClient::maintainConnection(bool distantNetworkInterrupted /*= false*/)
		#else
			void DistantClient::maintainConnection()
		#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			{
				// "onConnectionInterruptedForwarded()" est appel� lorsque le r�seau est int�rrompu
				// "onConnectionResumed()" est appel� lorsque le r�seau � repris
				mLastKeepAlive = Utils::Now();
			#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				if (distantNetworkInterrupted)
					onConnectionInterruptedForwarded();
				else
					onConnectionResumed();
			#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			}

			void DistantClient::onConnected()
			{
				mState = State::Connected;
				maintainConnection();
				onMessageReady(std::make_unique<Messages::Connection>(mAddress, mClientId, Messages::Connection::Result::Success));
				// Dispatch pending messages now
				for (auto&& pendingMessage : mPendingMessages)
				{
					onMessageReady(std::move(pendingMessage));
				}
			}

			void DistantClient::onDisconnectionFromOtherEnd()
			{
				if (isConnecting())
				{
					onConnectionRefused();
				}
				else if (isConnected())
				{
					mState = State::Disconnecting;
					mDisconnectionReason = DisconnectionReason::DisconnectedFromOtherEnd;
				}
			}

			void DistantClient::onConnectionLost()
			{
				// Start disconnecting and save the reason to notify later
				mState = State::Disconnecting;
				mDisconnectionReason = DisconnectionReason::Lost;
			}

			void DistantClient::onConnectionRefused()
			{
				if (mState == State::ConnectionSent)
				{
					mDisconnectionReason = DisconnectionReason::Refused;
				}
				mState = State::Disconnecting;
			}

			void DistantClient::onConnectionTimedOut()
			{
				if (mState == State::ConnectionSent)
				{
					mDisconnectionReason = DisconnectionReason::ConnectionTimedOut;
				}
				mState = State::Disconnecting;
			}

			void DistantClient::onConnectionInterrupted()
			{
			#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				if (mClient.isNetworkInterruptionAllowed())
				{
					if (!mInterrupted)
					{
						mInterrupted = true;
						mClient.onClientInterrupted(this);
						onMessageReady(std::make_unique<Messages::ConnectionInterrupted>(mAddress, mClientId, true));
					}
				}
				else
			#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				{
					if (isConnected())
					{
						onConnectionLost();
					}
				}
			}

			#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			void DistantClient::onConnectionInterruptedForwarded()
			{
				if (mClient.isNetworkInterruptionAllowed())
				{
					if (!mDistantInterrupted)
					{
						// If we were interrupted, we must have received something to know the distant interruption
						if (mInterrupted)
						{
							mInterrupted = false;
							onMessageReady(std::make_unique<Messages::ConnectionResumed>(mAddress, mClientId, false));
						}
						mDistantInterrupted = true;
						mClient.onClientInterrupted(this);
						onMessageReady(std::make_unique<Messages::ConnectionInterrupted>(mAddress, mClientId, false));
					}
				}
			}

			void DistantClient::onConnectionResumed()
			{
				if (mInterrupted || mDistantInterrupted)
				{
					mInterrupted = false;
					mDistantInterrupted = false;
					mClient.onClientResumed(this);
					onMessageReady(std::make_unique<Messages::ConnectionResumed>(mAddress, mClientId, !mClient.isNetworkInterrupted()));
				}
			}
			#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED

			void DistantClient::connect()
			{
				onConnectionSent();
			}
			void DistantClient::disconnect()
			{
				// To disconnect, stop sending packets
				// We switch to a disconnecting state to prevent a reconnection from happening
				// when receiving packets from the other end right after disconnecting locally
				mDisconnectionReason = DisconnectionReason::Disconnected;
				mState = State::Disconnecting;
				mLastKeepAlive = Utils::Now();
			}

			// permet d'envoyer des donn�es au client
			void DistantClient::send(std::vector<uint8>&& data, uint32 channelIndex)
			{
				onConnectionSent();
				mChannelsHandler.queue(std::move(data), channelIndex);
			}

			// permet de remplir l'en-t�te du datagram avant de l'envoyer
			void DistantClient::fillDatagramHeader(Datagram& dgram, Datagram::Type type)
			{
				dgram.header.ack = htons(mReceivedAcks.lastAck());
				dgram.header.previousAcks = mReceivedAcks.previousAcksMask();
				dgram.header.id = htons(mNextDatagramIdToSend);
				dgram.header.type = type;
				++mNextDatagramIdToSend;
			}

			void DistantClient::send(const Datagram& dgram)
			{
				int ret = mAddress.sendTo(mClient.mSocket, reinterpret_cast<const char*>(&dgram), dgram.size());
				if (ret < 0)
				{
					// Error
				}
			}

			// g�re l'envoie de donn�es et les diff�rents timeout
			void DistantClient::processSend(const uint8 maxDatagrams /*= 0*/)
			{
				const auto now = Utils::Now();
				// We do send data during connection process in order to keep it available before we accept it
				if (isConnecting() || isConnected())
				{
#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
					if (mClient.isNetworkInterrupted())
					{
						// Since the network is interrupted, send a keep alive to let the client know that
						Datagram datagram;
						fillKeepAlive(datagram);
						send(datagram);
					}
#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
					for (size_t loop = 0; maxDatagrams == 0 || loop < maxDatagrams; ++loop)
					{
						Datagram datagram;
						datagram.datasize = mChannelsHandler.serialize(datagram.data.data(), Datagram::DataMaxSize, mNextDatagramIdToSend
#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
							, mClient.isNetworkInterrupted()
#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
						);
						if (datagram.datasize > 0)
						{
							fillDatagramHeader(datagram, Datagram::Type::ConnectedData);
							send(datagram);
						}
						else
						{
							const bool sendKeepAlive = (loop == 0)
#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
								&& !mClient.isNetworkInterrupted()
#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
								;
							if (sendKeepAlive)
							{
								// Nothing to send this time, so send a keep alive to maintain connection
								fillKeepAlive(datagram);
								send(datagram);
							}
							// Everything has been sent. Exit the sending loop.
							break;
						}
					}
				}
				if (isDisconnecting())
				{
					if (now > mLastKeepAlive + 2 * GetTimeout())
					{
						// After 2 timeouts we mark it disconnected
						// This leaves enough time to each end to notice and disconnects its distant client
#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
	// Resume it right before disconnection for the app to receive the corresponding messages
						if (isInterrupted())
							onConnectionResumed();
#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
						// Do notify the disconnection, if needed
						// We notify it as latest as possible so when user received the Disconnection message, he can send a new connection request right away
						switch (mDisconnectionReason)
						{
						case DisconnectionReason::Disconnected:
						case DisconnectionReason::DisconnectedFromOtherEnd:
							onMessageReady(std::make_unique<Messages::Disconnection>(mAddress, mClientId, Messages::Disconnection::Reason::Disconnected));
							break;
						case DisconnectionReason::Lost:
							onMessageReady(std::make_unique<Messages::Disconnection>(mAddress, mClientId, Messages::Disconnection::Reason::Lost));
							break;
						case DisconnectionReason::Refused:
							onMessageReady(std::make_unique<Messages::Connection>(mAddress, mClientId, Messages::Connection::Result::Refused));
							break;
						case DisconnectionReason::ConnectionTimedOut:
							onMessageReady(std::make_unique<Messages::Connection>(mAddress, mClientId, Messages::Connection::Result::TimedOut));
							break;
						}
						mState = State::Disconnected;
					}
					else if (mDisconnectionReason != DisconnectionReason::None && mDisconnectionReason != DisconnectionReason::Lost)
					{
						// Send disconnection datagrams while disconnecting to inform the other end that's a normal termination
						Datagram datagram;
						fillDatagramHeader(datagram, Datagram::Type::Disconnection);
						send(datagram);
					}
				}
				else if (isConnected() && now > mLastKeepAlive + GetTimeout())
				{
					onConnectionInterrupted();
				}
				else if (isConnecting() && now > mConnectionStartTime + GetTimeout())
				{
					// Connection hasn't been accepted within timeframe : drop it, we don't keep an interrupted connection before it's been accepted
					onConnectionTimedOut();
				}
			}

			void DistantClient::fillKeepAlive(Datagram& dgram)
			{
				fillDatagramHeader(dgram, Datagram::Type::KeepAlive);
				// Do notify the other end if we're supposed to be connected or requesting the connection
				Serialization::Serializer serializer;
				serializer.write(mState == State::ConnectionSent || isConnected());
#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				// If the connection is interrupted with that client, check whether another client is also interrupting it or not
				// If this client is the only one causing the interruption, don't send him an interruption flag so he doesn't interrupt himself when resuming on his side
				const bool isNetworkInterrupted = mClient.isNetworkInterrupted();
				const bool isNetworkInterruptedByMe = mClient.isInterruptionCulprit(this);
				serializer.write(isNetworkInterrupted && !isNetworkInterruptedByMe);
#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				memcpy(dgram.data.data(), serializer.buffer(), serializer.bufferSize());
				dgram.datasize = static_cast<uint16>(serializer.bufferSize());
			}

			// permet de traiter les messages de maintien de connexion du client
			void DistantClient::handleKeepAlive(const uint8* data, const uint16 datasize)
			{
				Serialization::Deserializer deserializer(data, datasize);
				bool isConnectedKeepAlive = false;
				if (deserializer.read(isConnectedKeepAlive) && isConnectedKeepAlive)
				{
					if (mState == State::None || isConnecting())
					{
						onConnectionReceived();
					}
				}

			#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				bool isNetworkInterruptedOnTheOtherEnd = false;
				// Retrieve whether the other side has its connection interrupted and we should locally interrupt it too
				deserializer.read(isNetworkInterruptedOnTheOtherEnd);
				// Always consider the connection OK when a keep alive is received, but do keep in mind the network may be interrupted because it's interrupted on the other end.
				maintainConnection(isNetworkInterruptedOnTheOtherEnd);
			#else
				maintainConnection();
			#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			}

			// fonction appel� lorsqu'un datagram est re�u c�t� client
			void DistantClient::onDatagramReceived(Datagram&& datagram)
			{
				const auto datagramid = ntohs(datagram.header.id);
				//!< Update the received acks tracking
				mReceivedAcks.update(datagramid, 0, true);
				//!< Update the send acks tracking
				mSentAcks.update(ntohs(datagram.header.ack), datagram.header.previousAcks, true);
				//!< Ignore duplicate
				if (!mReceivedAcks.isNewlyAcked(datagramid))
				{
					return;
				}

				//!< Handle loss on reception
				const auto lostDatagrams = mReceivedAcks.loss();
				for (const auto receivedLostDatagram : lostDatagrams)
				{
					onDatagramReceivedLost(receivedLostDatagram);
				}
				//!< Handle loss on send
				const auto datagramsSentLost = mSentAcks.loss();
				for (const auto sendLoss : datagramsSentLost)
				{
					onDatagramSentLost(sendLoss);
				}
				//!< Mark new send acked
				const auto datagramsSentAcked = mSentAcks.getNewAcks();
				for (const auto sendAcked : datagramsSentAcked)
				{
					onDatagramSentAcked(sendAcked);
				}
				switch (datagram.header.type)
				{
					case Datagram::Type::ConnectedData:
					{
						//!< Dispatch data
						onDataReceived(datagram.data.data(), datagram.datasize);
					} break;
					case Datagram::Type::KeepAlive:
					{
						handleKeepAlive(datagram.data.data(), datagram.datasize);
					} break;
					case Datagram::Type::Disconnection:
					{
						onDisconnectionFromOtherEnd();
					} break;
				}
			}

			// permet de g�rer les datagrams envoy�s
			void DistantClient::onDatagramSentAcked(Datagram::ID datagramId)
			{
				mChannelsHandler.onDatagramAcked(datagramId);
			}

			// permet de g�rer les datagrams perdus
			void DistantClient::onDatagramSentLost(Datagram::ID datagramId)
			{
				mChannelsHandler.onDatagramLost(datagramId);
			}

			// permet de g�rer les datagrams re�us
			void DistantClient::onDatagramReceivedLost(Datagram::ID) {}

			void DistantClient::onDataReceived(const uint8* data, const uint16 datasize)
			{
				mChannelsHandler.onDataReceived(data, datasize);
				auto receivedMessages = mChannelsHandler.process(isConnected());
				for (auto&& [channelId, msg] : receivedMessages)
				{
					onMessageReady(std::make_unique<Messages::UserData>(mAddress, mClientId, std::move(msg), channelId));
				}
			}

			// permet de traiter les messages pr�t � �tre envoyer ou re�us
			void DistantClient::onMessageReady(std::unique_ptr<Messages::Base>&& msg)
			{
				if (isConnecting())
				{
					mPendingMessages.push_back(std::move(msg));
				}
				else if (isConnected() || isDisconnecting())
				{
					mClient.onMessageReady(std::move(msg));
				}
			}
		}
	}
}
