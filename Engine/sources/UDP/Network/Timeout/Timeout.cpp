#include "UDP/SocketsUDP.h"
#include "UDP/ClientUDP.h"
#include "UDP/Protocol/ReliableOrdered.h"
#include "UDP/Serialization/Deserializer.h"
#include "UDP/Serialization/Serializer.h"
#include "UDP/Messages.h"
#include "UDP/ErrorsUDP.h"

#include <iostream>
#include <mutex>
#include <string>
#include <thread>

int Timeout()
{
	if (Bousk::Network::Start())
	{
		std::cout << "Erreur d'init : " << Bousk::Network::ErrorsUDP::Get();
		return -1;
	}

	const Bousk::Network::Address client1 = Bousk::Network::Address::Loopback(Bousk::Network::Address::Type::IPv4, 8888);
	const Bousk::Network::Address client2 = Bousk::Network::Address::Loopback(Bousk::Network::Address::Type::IPv4, 9999);

	std::mutex coutMutex;
	std::thread t1([&]()
		{
			Bousk::Network::UDP::ClientUDP client;
			//client.registerChannel<Bousk::Network::UDP::Protocols::ReliableOrdered>();
			if (!client.init(client1.port()))
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 erreur d'initialisation: " << Bousk::Network::ErrorsUDP::Get();
				return;
			}
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 initialis� sur le port " << client1.port() << std::endl;
			}
			// Connecter client 1 � client 2
			client.connect(client2);
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 connexion � " << client2.toString() << "en cours..." << std::endl;
			}
			std::vector<std::string> receivedMessages;
			for (bool exit = false; !exit;)
			{
				client.receive();
				auto messages = client.poll();
				for (auto&& message : messages)
				{
					if (message->is<Bousk::Network::Messages::Connection>())
					{
						if (message->emitter() != client2)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Connexion inattendue de " << message->emitter().toString() << " (devait �tre de " << client2.toString() << ")" << std::endl;
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 2 [" << client2.toString() << "] connect� � client 1" << std::endl;
							std::cout << "Extinction de client 1..." << std::endl;
							exit = true;
						}
					}
				}
				client.processSend();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
			client.release();
		});
		std::thread t2([&]()
		{
			Bousk::Network::UDP::ClientUDP client;
			//client.registerChannel<Bousk::Network::UDP::Protocols::ReliableOrdered>();
			if (!client.init(client2.port()))
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 2 erreur d'init : " << Bousk::Network::ErrorsUDP::Get();
				return;
			}
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 2 initialis� sur le port " << client2.port() << std::endl;
			}
			for (bool exit = false; !exit;)
			{
				client.receive();
				auto messages = client.poll();
				for (auto&& message : messages)
				{
					if (message->is<Bousk::Network::Messages::IncomingConnection>())
					{
						if (message->emitter() != client1) {
							std::scoped_lock lock(coutMutex);
							std::cout << "Connexion entrante inattendue de " << message->emitter().toString() << " (devrait �tre " << client1.toString() << ")" << std::endl;
							client.disconnect(message->emitter());
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 2 re�oit une demande de connexion de [" << message->emitter().toString() << "] (client 1)... et l�accepte" << std::endl;
						}
						client.connect(message->emitter());
					}
					else if (message->is<Bousk::Network::Messages::Connection>())
					{
						if (message->emitter() != client1)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Connexion inattendue de " << message->emitter().toString() << " (devrait �tre " << client1.toString() << ")" << std::endl;
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 1 [" << client1.toString() << "] connect� � client 2" << std::endl;
						}
					}
					else if (message->is<Bousk::Network::Messages::Disconnection>())
					{
						std::scoped_lock lock(coutMutex);
						assert(message->emitter() == client1);
						std::cout << "D�connexion de client 1... [" << static_cast<int>(message->as<Bousk::Network::Messages::Disconnection>()->reason) << "]" << std::endl;
						exit = true;
					}
				}
				client.processSend();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			std::cout << "Fin de processus normal" << std::endl;
			client.release();
		});

	t1.join();
	t2.join();

	Bousk::Network::Release();
	return 0;
}