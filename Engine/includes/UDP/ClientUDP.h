#pragma once
#include "UDP/Messages.h"
#include <TCP/Sockets.h>
#include <TCP/Errors.h>
#include "UDP/Address.h"
#include "UDP/Datagram.h"
#include "UDP/DistantClient.h"
#include "WinSock2.h"
#include <vector>
#include <cassert>
#include <memory>

class ClientUDP
{
	friend class DistantClient;

public:
	ClientUDP();
	ClientUDP(const ClientUDP&) = delete;
	ClientUDP(ClientUDP&&) = delete;
	ClientUDP& operator=(const ClientUDP&) = delete;
	ClientUDP& operator=(ClientUDP&&) = delete;
	~ClientUDP();

	bool init(uint16_t port);
	void release();
	void sendTo(SOCKET _mSocket, std::vector<uint8_t>&& data);
	void receive();
	void onMessageReceived(std::unique_ptr<Bousk::Messages::Base>&& msg);
	DistantClient& getClient(const sockaddr_storage& clientAddr);
	void sendTo(const sockaddr_storage& target, std::vector<uint8_t>&& data);

	std::vector<std::unique_ptr<Bousk::Messages::Base>> poll();
	SOCKET mSocket{ INVALID_SOCKET };
	std::vector< std::unique_ptr<DistantClient>> mClients;
	std::vector < std::unique_ptr < Bousk:: Messages::Base >> mMessages;
	Bousk::Network::Address address;
};

