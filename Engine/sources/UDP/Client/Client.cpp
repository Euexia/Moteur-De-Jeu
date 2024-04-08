//#include "UDP/Client/Client.h"
//
//namespace Network
//{
//	namespace UDP
//	{
//		bool ConnectionHandlerUDP::connect(SOCKET sckt, const std::string& address, unsigned short port)
//		{
//			assert(sckt != INVALID_SOCKET);
//			mAddress = address;
//			mPort = port;
//			// permet d'�couter les diff�rents �v�nements ( voir en dessous le Message::Connection )
//			mFd.fd = sckt;
//			// de base POLLOUT - il v�rifiera si le descripteur de fichier est pr�t � �crire.
//			mFd.events = POLLOUT;
//			inet_pton(AF_INET, mAddress.c_str(), &mConnectedAddress.sin_addr.s_addr);
//			mConnectedAddress.sin_family = AF_INET;
//			mConnectedAddress.sin_port = htons(mPort);
//			// tente de ce connecter au serveur distant
//			if (::connect(sckt, (const sockaddr*)&mConnectedAddress, sizeof(mConnectedAddress)) != 0)
//			{
//				int err = Errors::Get();
//				if (err != Errors::INPROGRESS && err != Errors::WOULDBLOCK)
//					return false;
//			}
//			return true;
//		}
//		// Nous utilisons "poll()" pour ne pas bloquer le programme quand une op�ration UDP est en cours
//		std::unique_ptr<Messages::Connection> ConnectionHandlerUDP::poll()
//		{
//			int res = ::poll(&mFd, 1, 0);
//			if (res < 0)
//				return std::make_unique<Messages::Connection>(Messages::Connection::Result::Failed);
//			else if (res > 0)
//			{
//				// Descripteur de fichier permet d'identifier de mani�re unique chaque fichier ouvert
//				if (mFd.revents & POLLOUT)
//				{
//					// Descripteur de fichier est pr�t � �tre �crit
//					return std::make_unique<Messages::Connection>(Messages::Connection::Result::Success);
//				}
//				else if (mFd.revents & (POLLHUP | POLLNVAL))
//				{
//					// Fin de connexion ou descripteur de fichier invalide
//					return std::make_unique<Messages::Connection>(Messages::Connection::Result::Failed);
//				}
//				else if (mFd.revents & POLLERR)
//				{
//					// Erreur sur le descripteur de fichier
//					return std::make_unique<Messages::Connection>(Messages::Connection::Result::Failed);
//				}
//			}
//			//!< action non termin�e
//			return nullptr;
//		}
//
//		void ReceptionHandlerUDP::init(SOCKET sckt)
//		{
//			assert(sckt != INVALID_SOCKET);
//			mSckt = sckt;
//			startHeaderReception();
//		}
//		// init la r�c�ption du Header ( l'en-t�te ) en prenant compte de sa taille
//		void ReceptionHandlerUDP::startHeaderReception()
//		{
//			startReception(HeaderSize, State::Header);
//		}
//		// fonction appell�e une fois que qu'elle � re�u le Header complet
//		void ReceptionHandlerUDP::startDataReception()
//		{
//			assert(mBuffer.size() == sizeof(HeaderType));
//			HeaderType networkExpectedDataLength;
//			memcpy(&networkExpectedDataLength, mBuffer.data(), sizeof(networkExpectedDataLength));
//			const auto expectedDataLength = ntohs(networkExpectedDataLength);
//			startReception(expectedDataLength, State::Data);
//		}
//		void ReceptionHandlerUDP::startReception(unsigned int expectedDataLength, State newState)
//		{
//			mReceived = 0;
//			mBuffer.clear();
//			mBuffer.resize(expectedDataLength, 0);
//			mState = newState;
//		}
//		std::unique_ptr<Messages::Base> ReceptionHandlerUDP::recvfrom()
//		{
//			assert(mSckt != INVALID_SOCKET);
//
//			sockaddr_in senderAddr;
//			int senderAddrSize = sizeof(senderAddr);
//			int ret = ::recvfrom(mSckt, missingDataStartBuffer(), missingDataLength(), 0, reinterpret_cast<sockaddr*>(&senderAddr), &senderAddrSize);
//			if (ret > 0)
//			{
//				mReceived += ret;
//				if (mReceived == mBuffer.size())
//				{
//					if (mState == State::Data)
//					{
//						// si toutes les donn�es attendues sont arriv�es alors elle renvoie "Messages::UserData"
//						std::unique_ptr<Messages::Base> msg = std::make_unique<Messages::UserData>(std::move(mBuffer));
//						startHeaderReception();
//						return msg;
//					}
//					else
//					{
//						startDataReception();
//						//!< si jamais les donn�es sont d�j� disponibles elles seront ainsi retourn�es directement
//						return recvfrom();
//					}
//				}
//				return nullptr;
//			}
//			else if (ret == 0)
//			{
//				//!< connexion termin�e correctement
//				return std::make_unique<Messages::Disconnection>(Messages::Disconnection::Reason::Disconnected);
//			}
//			else // ret < 0
//			{
//				//!< traitement d'erreur
//				int error = Errors::Get();
//				if (error == Errors::WOULDBLOCK || error == Errors::AGAIN)
//				{
//					return nullptr;
//				}
//				else
//				{
//					return std::make_unique<Messages::Disconnection>(Messages::Disconnection::Reason::Lost);
//				}
//			}
//		}
//		
//		void SendingHandlerUDP::init(SOCKET sckt)
//		{
//			mSocket = sckt;
//			if (mState == State::Header || mState == State::Data)
//			{
//				// nettoie le tampon d'envoie
//				mSendingBuffer.clear();
//			}
//			mState = State::Idle;
//		}
//
//		// permet de mettre des donn�es dans la file d'attente pour l'envoie
//		bool SendingHandlerUDP::sendto(const unsigned char* data, unsigned int datalen)
//		{
//			if (datalen > std::numeric_limits<HeaderType>::max())
//			{
//				return false;
//			}
//			mQueueingBuffers.emplace_back(data, data + datalen);
//			return true;
//		}
//
//		// permet de g�rer l'envoie de donn�es
//		void SendingHandlerUDP::update()
//		{
//			assert(mSocket != INVALID_SOCKET);
//			if (mState == State::Idle && !mQueueingBuffers.empty())
//			{
//				// si il y a des donn�es en attente, elle pr�pare l'envoie du prochain en-t�te
//				prepareNextHeader();
//			}
//			// tant on peut envoyer des donn�es
//			while (mState != State::Idle && sendPendingBuffer())
//			{
//				if (mState == State::Header)
//				{
//					prepareNextData();
//				}
//				else
//				{
//					if (!mQueueingBuffers.empty())
//					{
//						prepareNextHeader();
//					}
//					else
//					{
//						mState = State::Idle;
//					}
//				}
//			}
//		}
//
//		// permet d'envoyer des donn�es qui sont en attente sur le socket
//		bool SendingHandlerUDP::sendPendingBuffer()
//		{
//			if (mSendingBuffer.empty())
//			{
//				return true;
//			}
//
//			//!< envoi des donn�es restantes du dernier envoi
//			int sent = ::sendto(mSocket, reinterpret_cast<char*>(mSendingBuffer.data()), static_cast<int>(mSendingBuffer.size()), 0, nullptr, 0);
//			if (sent > 0)
//			{
//				if (sent == mSendingBuffer.size())
//				{
//					//!< toutes les donn�es ont �t� envoy�es
//					mSendingBuffer.clear();
//					return true;
//				}
//				else
//				{
//					//!< envoi partiel
//					memmove(mSendingBuffer.data() + sent, mSendingBuffer.data(), sent);
//					mSendingBuffer.erase(mSendingBuffer.cbegin() + sent, mSendingBuffer.cend());
//				}
//			}
//			return false;
//		}
//
//		// pr�pare l'envoie de la prochaine en-t�te
//		void SendingHandlerUDP::prepareNextHeader()
//		{
//			assert(!mQueueingBuffers.empty());
//			const auto header = static_cast<HeaderType>(mQueueingBuffers.front().size());
//			const auto networkHeader = htons(header);
//			mSendingBuffer.clear();
//			mSendingBuffer.resize(HeaderSize);
//			memcpy(mSendingBuffer.data(), &networkHeader, sizeof(HeaderType));
//			mState = State::Header;
//		}
//		
//		// pr�pare l'envoie des prochaines donn�es
//		void SendingHandlerUDP::prepareNextData()
//		{
//			assert(!mQueueingBuffers.empty());
//			mSendingBuffer.swap(mQueueingBuffers.front());
//			mQueueingBuffers.pop_front();
//			mState = State::Data;
//		}
//
//		// permet de calculer la taille total des donn�es en attente
//		size_t SendingHandlerUDP::queueSize() const
//		{
//			size_t s = std::accumulate(mQueueingBuffers.cbegin(), mQueueingBuffers.cend(), static_cast<size_t>(0), [](size_t n, const std::vector<unsigned char>& queuedItem) {
//				return n + queuedItem.size() + HeaderSize;
//				});
//			if (mState == State::Data)
//				s += mSendingBuffer.size();
//			return s;
//		}
//
//		ClientImplUDP::~ClientImplUDP()
//		{
//			disconnect();
//		}
//
//		bool ClientImplUDP::init(SOCKET&& sckt, const sockaddr_in& addr)
//		{
//			assert(sckt != INVALID_SOCKET);
//			if (sckt == INVALID_SOCKET)
//				return false;
//
//			assert(mState == State::Disconnected);
//			assert(mSocket == INVALID_SOCKET);
//			if (mSocket != INVALID_SOCKET)
//				disconnect();
//
//			mSocket = sckt;
//			if (!SetNonBlocking(mSocket))
//			{
//				disconnect();
//				return false;
//			}
//			onConnected(addr);
//			return true;
//		}
//
//		bool ClientImplUDP::connect(const std::string& ipaddress, unsigned short port)
//		{
//			assert(mState == State::Disconnected);
//			assert(mSocket == INVALID_SOCKET);
//			if (mSocket != INVALID_SOCKET)
//			{
//				disconnect();
//			}
//			mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//			if (mSocket == INVALID_SOCKET)
//			{
//				return false;
//			}
//			else if (!SetNonBlocking(mSocket))
//			{
//				disconnect();
//				return false;
//			}
//			if (mConnectionHandler.connect(mSocket, ipaddress, port))
//			{
//				mState = State::Connecting;
//				return true;
//			}
//			return false;
//		}
//
//		void ClientImplUDP::disconnect()
//		{
//			if (mSocket != INVALID_SOCKET)
//			{
//				CloseSocket(mSocket);
//			}
//			mSocket = INVALID_SOCKET;
//			mState = State::Disconnected;
//		}
//
//		bool ClientImplUDP::sendto(const unsigned char* data, unsigned int len)
//		{
//			return mSendingHandler.sendto(data, len);
//		}
//
//		std::unique_ptr<Messages::Base> ClientImplUDP::poll()
//		{
//			switch (mState)
//			{
//			// check si le client ce connecte
//			case State::Connecting:
//			{
//				// si il ce connecte alors "mConnectionHandler.poll()" v�rifie l'�tat de la connexion qui est en cours
//				auto msg = mConnectionHandler.poll();
//				if (msg)
//				{
//					// connexion r�ussite
//					if (msg->result == Messages::Connection::Result::Success)
//					{
//						onConnected(mConnectionHandler.connectedAddress());
//					}
//					else
//					{
//						// fermer la connexion car il y a eu une erreur
//						disconnect();
//					}
//				}
//				// on return quand m�me le message
//				return msg;
//			} break;
//			// le client est d�j� connect�
//			case State::Connected:
//			{
//				// appelle "update" pour envoyer les donn�es en attente
//				mSendingHandler.update();
//				// recvfrom qui sert � recevoir les messages entrant
//				auto msg = mReceivingHandler.recvfrom();
//				if (msg)
//				{
//					if (msg->is<Messages::Disconnection>())
//					{
//						disconnect();
//					}
//				}
//				return msg;
//			} break;
//			case State::Disconnected:
//			{
//			} break;
//			}
//			return nullptr;
//		}
//
//		// connexion r�ussie, init de l'adresse du serveur + l'init des gestionnaires d'envoi et de r�ception
//		void ClientImplUDP::onConnected(const sockaddr_in& addr)
//		{
//			mAddress = addr;
//			mSendingHandler.init(mSocket);
//			mReceivingHandler.init(mSocket);
//			mState = State::Connected;
//		}
//
//		// constructeur de d�placement & op�rateur d'affectation de d�placement
//		// Ils s'occupent de transferer les ressources ( mImpl ) de l'objet source vers l'objet this
//		// Utilisation de std::move() pour �viter les copies co�teuses
//		ClientUDP::ClientUDP(ClientUDP&& other)
//			: mImpl(std::move(other.mImpl))
//		{}
//		ClientUDP& ClientUDP::operator=(ClientUDP&& other)
//		{
//			mImpl = std::move(other.mImpl);
//			return *this;
//		}
//
//		// init permet d'initialiser une connexion � un serveur distant
//		bool ClientUDP::init(SOCKET&& sckt, const sockaddr_in& addr)
//		{
//			if (!mImpl)
//				mImpl = std::make_unique<ClientImplUDP>();
//			return mImpl && mImpl->init(std::move(sckt), addr);
//		}
//
//		// connect permet d'�tablir la connexion � un serveur distant
//		bool ClientUDP::connect(const std::string& ipaddress, unsigned short port)
//		{
//			if (!mImpl)
//				mImpl = std::make_unique<ClientImplUDP>();
//			return mImpl && mImpl->connect(ipaddress, port);
//		}
//
//		// disconnect permet de fermer la connexion avec le serveur distant
//		void ClientUDP::disconnect() { if (mImpl) mImpl->disconnect(); mImpl = nullptr; }
//
//		// send permet d'envoyer des donn�es au serveur distant gr�ce � la connexion �tablie
//		bool ClientUDP::sendto(const unsigned char* data, unsigned int len) { return mImpl && mImpl->sendto(data, len); }
//
//		// poll permet de v�rifier si les donn�es venant du serveur distant sont disponibles � recevoir
//		// Renvoie "Messages::Base" pour d�terminer le messages � transmettre
//		std::unique_ptr<Messages::Base> ClientUDP::poll() { return mImpl ? mImpl->poll() : nullptr; }
//
//		//permet d'obtenir l'id du client et l'adresse de destination associ�e � la connexion
//		uint64_t ClientUDP::id() const { return mImpl ? mImpl->id() : -1; }
//		const sockaddr_in& ClientUDP::destinationAddress() const { static sockaddr_in empty{ 0 }; return mImpl ? mImpl->destinationAddress() : empty; }
//	}
//}