//#include "UDP/Server/Server.h"
//
//namespace Network
//{
//	namespace UDP
//	{
//		ServerImplUDP::~ServerImplUDP()
//		{
//			stop();
//		}
//
//		bool ServerImplUDP::start(unsigned short _port)
//		{
//			assert(mSocket == INVALID_SOCKET);
//			if (mSocket != INVALID_SOCKET)
//				stop();
//			mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//			if (mSocket == INVALID_SOCKET)
//				return false;
//
//			if (!SetReuseAddr(mSocket) || !SetNonBlocking(mSocket))
//			{
//				stop();
//				return false;
//			}
//
//			sockaddr_in addr;
//			addr.sin_addr.s_addr = INADDR_ANY;
//			addr.sin_port = htons(_port);
//			addr.sin_family = AF_INET;
//			if (bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
//			{
//				stop();
//				return false;
//			}
//			if (listen(mSocket, SOMAXCONN) != 0)
//			{
//				stop();
//				return false;
//			}
//			return true;
//		}
//		void ServerImplUDP::stop()
//		{
//			for (auto& client : mClients)
//				client.second.disconnect();
//			mClients.clear();
//			if (mSocket != INVALID_SOCKET)
//				CloseSocket(mSocket);
//			mSocket = INVALID_SOCKET;
//		}
//		void ServerImplUDP::update()
//		{
//			if (mSocket == INVALID_SOCKET)
//				return;
//
//			//!< accept jusqu'à 10 nouveaux clients
//			for (int accepted = 0; accepted < 10; ++accepted)
//			{
//				sockaddr_in addr = { 0 };
//				socklen_t addrlen = sizeof(addr);
//				SOCKET newClientSocket = accept(mSocket, reinterpret_cast<sockaddr*>(&addr), &addrlen);
//				if (newClientSocket == INVALID_SOCKET)
//					break;
//				ClientUDP newClient;
//				if (newClient.init(std::move(newClientSocket), addr))
//				{
//					auto message = std::make_unique<Messages::Connection>(Messages::Connection::Result::Success);
//					message->idFrom = newClient.id();
//					message->from = newClient.destinationAddress();
//					mMessages.push_back(std::move(message));
//					mClients[newClient.id()] = std::move(newClient);
//				}
//			}
//
//			//!< mise à jour des clients connectés
//			//!< réceptionne au plus 1 message par client
//			//!< supprime de la liste les clients déconnectés
//			for (auto itClient = mClients.begin(); itClient != mClients.end(); )
//			{
//				auto& client = itClient->second;
//				auto msg = client.poll();
//				if (msg)
//				{
//					msg->from = itClient->second.destinationAddress();
//					msg->idFrom = itClient->second.id();
//					if (msg->is<Messages::Disconnection>())
//					{
//						itClient = mClients.erase(itClient);
//					}
//					else
//						++itClient;
//					mMessages.push_back(std::move(msg));
//				}
//				else
//					++itClient;
//			}
//		}
//		bool ServerImplUDP::sendTo(uint64_t clientid, const unsigned char* data, unsigned int len)
//		{
//			auto itClient = mClients.find(clientid);
//			return itClient != mClients.end() && itClient->second.sendto(data, len);
//		}
//
//		bool ServerImplUDP::sendToAll(const unsigned char* data, unsigned int len)
//		{
//			bool ret = true;
//			for (auto& client : mClients)
//				ret &= client.second.sendto(data, len);
//			return ret;
//		}
//
//		std::unique_ptr<Messages::Base> ServerImplUDP::poll()
//		{
//			if (mMessages.empty())
//				return nullptr;
//
//			auto msg = std::move(mMessages.front());
//			mMessages.pop_front();
//			return msg;
//		}
//
//
//		// permet de déplacer les ressources de "other" vers l'instance de "Server"
//		ServerUDP::ServerUDP(ServerUDP&& other)
//			: mImpl(std::move(other.mImpl))
//		{}
//
//		ServerUDP& ServerUDP::operator=(ServerUDP&& other)
//		{
//			mImpl = std::move(other.mImpl);
//			return *this;
//		}
//
//		// permet de démarrer le serveur
//		bool ServerUDP::start(unsigned short _port)
//		{
//			if (!mImpl)
//				mImpl = std::make_unique<ServerImplUDP>();
//			return mImpl && mImpl->start(_port);
//		}
//
//		// permet de stopper le serveur
//		void ServerUDP::stop()
//		{
//			if (mImpl) mImpl->stop();
//		}
//
//		// permet de mettre à jour le serveur donc par exemple une connexion / deconnection ect...
//		void ServerUDP::update()
//		{
//			if (mImpl) mImpl->update();
//		}
//
//		// permet d'envoyer des données à un client spécifique grâce à son ID ( clientid )
//		bool ServerUDP::sendTo(uint64_t clientid, const unsigned char* data, unsigned int len)
//		{
//			return mImpl && mImpl->sendTo(clientid, data, len);
//		}
//
//		// permet d'envoyer des données à tous les clients connectés
//		bool ServerUDP::sendToAll(const unsigned char* data, unsigned int len)
//		{
//			return mImpl && mImpl->sendToAll(data, len);
//		}
//
//		// vérifie s'il y a des messages en attente
//		std::unique_ptr<Messages::Base> ServerUDP::poll()
//		{
//			return mImpl ? mImpl->poll() : nullptr;
//		}
//	}
//}