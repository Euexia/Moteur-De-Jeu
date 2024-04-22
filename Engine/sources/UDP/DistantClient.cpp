#include "UDP/DistantClient.h"

DistantClient::DistantClient(ClientUDP& client, const Bousk::Network::Address& addr)
{
}

DistantClient::~DistantClient()
{
}

void DistantClient::send(std::vector<uint8_t>&& data)
{
	Bousk::UDP::Datagram datagram;
	datagram.header.id = htons(mNextDatagramIdToSend);
	++mNextDatagramIdToSend;
	memcpy(datagram.data.data(), data.data(), data.size());

	datagram.header.ack = htons(mReceivedAcks.lastAck());
	datagram.header.previousAcks = mReceivedAcks.previousAcksMask();

	sendto(mClient.mSocket, reinterpret_cast<const char*>(&datagram), static_cast<int>(Bousk::UDP::Datagram::HeaderSize + data.size()), 0,
		reinterpret_cast<const sockaddr*>(&mAddress), sizeof(mAddress));
}

void DistantClient::onDatagramReceived(Bousk::UDP::Datagram&& datagram)
{
	const auto datagramid = ntohs(datagram.header.id);
	//!< Mise à jour des datagrammes reçus
	mReceivedAcks.Update(datagramid, datagram.header.previousAcks, true);
	//!< Ignorer les duplications
	if (!mReceivedAcks.isNewlyAcked(datagramid))
		return;
	//!< Gérer les données reçues
	onDataReceived(std::vector<uint8_t>(datagram.data.data(), datagram.data.data() + datagram.datasize));
	//!< Mise à jour du suivi des datagrammes envoyés et acquittés par l’autre partie
	mSentAcks.Update(ntohs(datagram.header.ack), datagram.header.previousAcks, true);

	//!< Traiter les pertes à la réception
	const auto lostDatagrams = mReceivedAcks.loss();
	for (const auto receivedLostDatagram : lostDatagrams)
	{
		onDatagramReceivedLost(receivedLostDatagram);
	}
	//!< Gérer les données envoyées et non reçue
	const auto datagramsSentLost = mSentAcks.loss();
	for (const auto sendLoss : datagramsSentLost)
	{
		onDatagramSentLost(sendLoss);
	}
	//!< Traiter les données envoyées et acquittées
	const auto datagramsSentAcked = mSentAcks.getNewAcks();
	for (const auto sendAcked : datagramsSentAcked)
	{
		onDatagramSentAcked(sendAcked);
	}
}


void DistantClient::onDataReceived(std::vector<uint8_t>&& data)
{
	onMessageReady(std::make_unique<Bousk::Messages::UserData>(std::move(data)));
}

void DistantClient::onMessageReady(std::unique_ptr<Bousk::Messages::Base>&& msg)
{
	memcpy(&(msg->from), &mAddress, sizeof(mAddress));
	mClient.onMessageReceived(std::move(msg));
}

void DistantClient::onDatagramSentAcked(Bousk::UDP::Datagram::ID datagramId)
{
}

void DistantClient::onDatagramReceivedLost(Bousk::UDP::Datagram::ID datagramId)
{
}

void DistantClient::onDatagramSentLost(Bousk::UDP::Datagram::ID datagramId)
{
}
