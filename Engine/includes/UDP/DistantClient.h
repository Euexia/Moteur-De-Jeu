#pragma once

#include "UDP/Datagram.h"
#include "UDP/ClientUDP.h"
#include "UDP/AckHandler.h"
#include "UDP/Address.h"
#include "UDP/Messages.h"

#include <vector>
#include <memory>


class DistantClient
{
public:
	DistantClient(ClientUDP& client, const Bousk::Network::Address& addr);
	DistantClient(const DistantClient&) = delete;
	DistantClient(DistantClient&&) = delete;
	DistantClient& operator=(const DistantClient&) = delete;
	DistantClient& operator=(DistantClient&&) = delete;
	~DistantClient();

	void send(std::vector<uint8_t>&& data);
	void onDatagramReceived(Bousk::UDP::Datagram&& datagram);
	void onDataReceived(std::vector<uint8_t>&& data);
	void onMessageReady(std::unique_ptr<Bousk::Messages::Base>&& msg);
	void onDatagramSentAcked(Bousk::UDP::Datagram::ID datagramId);
	void onDatagramReceivedLost(Bousk::UDP::Datagram::ID datagramId);
	void onDatagramSentLost(Bousk::UDP::Datagram::ID datagramId);
	inline const Bousk::Network::Address& address() const { return mAddress; }

	Bousk::UDP::Datagram::ID mNextDatagramIdToSend{ 0 }; // Identifiant du prochain datagramme à envoyer
	ClientUDP mClient;
	Bousk::Network::Address mAddress;
	Bousk::UDP::AckHandler mReceivedAcks;
	Bousk::UDP::AckHandler mSentAcks;
};

