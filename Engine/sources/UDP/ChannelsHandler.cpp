#include "UDP/ChannelsHandler.h"
#include "UDP/ChannelHeader.h"
#include "UDP/Protocol/ReliableOrdered.h"
#include "UDP/Protocol/UnreliableOrdered.h"


#include <cassert>
#include <iterator>

namespace Bousk
{
	namespace Network
	{
		namespace UDP
		{
			ChannelsHandler::ChannelsHandler() = default;
			ChannelsHandler::~ChannelsHandler() = default;

			// Multiplexer
			// Permet d'ajouter les donn�es � la file d'attente d'un canal sp�cifique
			void ChannelsHandler::queue(std::vector<uint8>&& msgData, const uint32 channelIndex)
			{
				assert(channelIndex < mChannels.size());
				mChannels[channelIndex]->queue(std::move(msgData));
			}

			// Place les donn�es s�rializ� par le protocole associ� au canal ( elle parcours chaque canaux )
			// Ensuite elle est place dans le tampon de sortie ( zone m�moire ou sont stock� temporairement les donn�es )
			// Pour finir on ajoute un en-t�te pour y ajout� des infos en plus ou un identifiant
			uint16 ChannelsHandler::serialize(uint8* buffer, const uint16 buffersize, const Datagram::ID datagramId
			#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
				, const bool connectionInterrupted
			#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
			)
			{
				uint16 remainingBuffersize = buffersize;
				for (uint32 channelIndex = 0; channelIndex < mChannels.size(); ++channelIndex)
				{
					Protocols::IProtocol* protocol = mChannels[channelIndex].get();

					uint8* const channelHeaderStart = buffer;
					uint8* const channelDataStart = buffer + ChannelHeader::Size;
					const uint16 channelAvailableSize = remainingBuffersize - ChannelHeader::Size;

					const uint16 serializedData = protocol->serialize(channelDataStart, channelAvailableSize, datagramId
					#if BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
						, connectionInterrupted
					#endif // BOUSKNET_ALLOW_NETWORK_INTERRUPTION == BOUSKNET_SETTINGS_ENABLED
					);
					assert(serializedData <= channelAvailableSize);
					if (serializedData)
					{
						// Data added, let's add the protocol header
						ChannelHeader* const channelHeader = reinterpret_cast<ChannelHeader*>(channelHeaderStart);
						channelHeader->channelIndex = channelIndex;
						channelHeader->datasize = static_cast<uint32>(serializedData);

						const uint16 channelTotalSize = serializedData + ChannelHeader::Size;
						buffer += channelTotalSize;
						remainingBuffersize -= channelTotalSize;
					}
				}
				return buffersize - remainingBuffersize;
			}

			// Permet d'informer les cannaux que les datagrames ont �t� re�us
			void ChannelsHandler::onDatagramAcked(const Datagram::ID datagramId)
			{
				for (auto& channel : mChannels)
				{
					channel->onDatagramAcked(datagramId);
				}
			}

			// Permet d'informer les cannaux que les datagrames ont �t� perdus
			void ChannelsHandler::onDatagramLost(const Datagram::ID datagramId)
			{
				for (auto& channel : mChannels)
				{
					channel->onDatagramLost(datagramId);
				}
			}

			// Demultiplexer
			// Reforme chaque donn�es re�us en parcourant tout les cannaux pour associer chaque donn�es au canal appropri�
			void ChannelsHandler::onDataReceived(const uint8* data, const uint16 datasize)
			{
				uint16 processedData = 0;
				while (processedData < datasize)
				{
					const ChannelHeader* channelHeader = reinterpret_cast<const ChannelHeader*>(data);
					if (processedData + channelHeader->datasize > datasize || channelHeader->datasize > Datagram::DataMaxSize)
					{
						// Malformed buffer
						return;
					}
					if (channelHeader->channelIndex >= mChannels.size())
					{
						// Channel id requested doesn't exist
						return;
					}
					mChannels[channelHeader->channelIndex]->onDataReceived(data + ChannelHeader::Size, channelHeader->datasize);
					const uint16 channelTotalSize = channelHeader->datasize + ChannelHeader::Size;
					data += channelTotalSize;
					processedData += channelTotalSize;
				}
			}

			std::vector<std::tuple<uint8, std::vector<uint8>>> ChannelsHandler::process(bool isConnected)
			{
				std::vector<std::tuple<uint8, std::vector<uint8>>> messages;
				for (auto& channel : mChannels)
				{
					std::vector<std::vector<uint8>> protocolMessages = channel->process();
					// If we're not connected, ignore and discard unreliable messages
					if (!protocolMessages.empty() && (channel->isReliable() || isConnected))
					{
						messages.reserve(messages.size() + protocolMessages.size());
						for (auto&& msg : protocolMessages)
						{
							messages.push_back(std::make_tuple(channel->channelId(), std::move(msg)));
						}
					}
				}
				return messages;
			}

			template<class T>
			void ChannelsHandler::registerChannel(uint8 channelId)
			{
				mChannels.push_back(std::make_unique<T>(channelId));
			}
		}
	}
}
