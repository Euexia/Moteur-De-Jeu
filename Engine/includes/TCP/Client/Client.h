#pragma once

#include "TCP/Sockets.h"
#include "TCP/Messages.h"
#include "TCP/Errors.h"

#include <string>
#include <memory>
#include <vector>
#include <list>
#include <cassert>
#include <numeric>

namespace Network
{
    namespace Messages {
        class Base;
    }
    namespace TCP
    {
        /**
         * @brief Type de données utilisé pour l'en-tête des messages.
         */
        using HeaderType = uint16_t;

        /**
         * @brief Taille de l'en-tête des messages en octets.
         */
        static const unsigned int HeaderSize = sizeof(HeaderType);

        /**
         * @brief Implémentation privée de la classe Client.
         */
        class ClientImpl;

        /**
         * @brief La classe Client représente un client TCP.
         */
        class Client
        {
        public:
            /**
             * @brief Constructeur par défaut de la classe Client.
             */
            Client() {}

            /**
             * @brief Constructeur de copie supprimé.
             */
            Client(const Client&) = delete;

            /**
             * @brief Opérateur d'assignation de copie supprimé.
             */
            Client& operator=(const Client&) = delete;

            /**
             * @brief Constructeur de déplacement de la classe Client.
             */
            Client(Client&&);

            /**
             * @brief Opérateur d'assignation par déplacement de la classe Client.
             */
            Client& operator=(Client&&);

            /**
             * @brief Destructeur de la classe Client.
             */
            ~Client() {}

            /**
             * @brief Initialise le client avec le socket et l'adresse spécifiés.
             * @param sckt Socket à associer au client.
             * @param addr Adresse à associer au client.
             * @return true si l'initialisation réussit, sinon false.
             */
            bool init(SOCKET&& sckt, const sockaddr_in& addr);

            /**
             * @brief Établit une connexion avec le serveur à l'adresse et au port spécifiés.
             * @param ipaddress Adresse IP du serveur.
             * @param port Port du serveur.
             * @return true si la connexion réussit, sinon false.
             */
            bool connect(const std::string& ipaddress, unsigned short port);

            /**
             * @brief Ferme la connexion avec le serveur.
             */
            void disconnect();

            /**
             * @brief Envoie des données au serveur.
             * @param data Pointeur vers les données à envoyer.
             * @param len Longueur des données à envoyer.
             * @return true si l'envoi réussit, sinon false.
             */
            bool send(const unsigned char* data, unsigned int len);

            /**
             * @brief Récupère les messages entrants du serveur.
             * @return Un pointeur unique vers le message reçu.
             */
            std::unique_ptr<Messages::Base> poll();

            /**
             * @brief Récupère l'identifiant du client.
             * @return L'identifiant du client.
             */
            uint64_t id() const;

            /**
             * @brief Récupère l'adresse de destination du client.
             * @return L'adresse de destination du client.
             */
            const sockaddr_in& destinationAddress() const;

        private:
            std::unique_ptr<ClientImpl> mImpl; /**< Implémentation privée du client. */
        };


        class ConnectionHandler
        {
        public:
            ConnectionHandler() = default;

            /**
             * @brief Établit une connexion avec l'adresse et le port spécifiés.
             * @param sckt Socket à utiliser pour la connexion.
             * @param address Adresse IP du serveur.
             * @param port Port du serveur.
             * @return true si la connexion réussit, sinon false.
             */
            bool connect(SOCKET sckt, const std::string& address, unsigned short port);

            /**
             * @brief Récupère les messages de connexion.
             * @return Un pointeur unique vers le message de connexion reçu.
             */
            std::unique_ptr<Messages::Connection> poll();

            /**
             * @brief Renvoie l'adresse à laquelle la connexion est établie.
             * @return L'adresse à laquelle la connexion est établie.
             */
            const sockaddr_in& connectedAddress() const { return mConnectedAddress; }

        private:
            pollfd mFd = { 0 }; /**< Permet d'associer le descripteur de fichier à la connexion. */
            sockaddr_in mConnectedAddress; /**< Adresse IP + port auquel la connexion est établie. */
            std::string mAddress; /**< Adresse IP + port auquel doit être établie la connexion. */
            unsigned short mPort; /**< Port auquel doit être établie la connexion. */
        };

        class ReceptionHandler
        {
            enum class State {
                Header, /**< L'en-tête du message qui contient des métadonnées sur le message. */
                Data, /**< Les données comme du texte, des images, etc. (ce que l'utilisateur enverra). */
            };
        public:
            ReceptionHandler() = default;

            /**
             * @brief Initialise le gestionnaire de réception avec le socket spécifié.
             * @param sckt Socket à utiliser pour la réception.
             */
            void init(SOCKET sckt);

            /**
             * @brief Reçoit les messages entrants.
             * @return Un pointeur unique vers le message reçu.
             */
            std::unique_ptr<Messages::Base> recv();

        private:
            /**
             * @brief Pointeur vers le début des données manquantes dans le tampon de réception.
             * @return Pointeur vers le début des données manquantes.
             */
            inline char* missingDataStartBuffer() { return reinterpret_cast<char*>(mBuffer.data() + mReceived); }

            /**
             * @brief Calcule la longueur des données manquantes dans le tampon de réception.
             * @return Longueur des données manquantes.
             */
            inline int missingDataLength() const { return static_cast<int>(mBuffer.size() - mReceived); }

            /**
             * @brief Démarre la réception de l'en-tête du message.
             */
            void startHeaderReception();

            /**
             * @brief Démarre la réception des données du message.
             */
            void startDataReception();

            /**
             * @brief Démarre la réception en spécifiant la longueur attendue des données.
             * @param expectedDataLength Longueur attendue des données.
             * @param newState Nouvel état de la réception.
             */
            void startReception(unsigned int expectedDataLength, State newState);

        private:
            std::vector<unsigned char> mBuffer; /**< Tampon de réception des données. */
            unsigned int mReceived; /**< Nombre de données déjà reçues. */
            SOCKET mSckt{ INVALID_SOCKET }; /**< Socket utilisé pour la réception. */
            State mState; /**< État actuel de la réception. */
        };


        class SendingHandler
        {
            enum class State {
                Idle, /**< État inactif. */
                Header, /**< État de l'en-tête. */
                Data, /**< État des données. */
            };
        public:
            SendingHandler() = default;

            /**
             * @brief Initialise le gestionnaire d'envoi avec le socket spécifié.
             * @param sckt Socket à utiliser pour l'envoi.
             */
            void init(SOCKET sckt);

            /**
             * @brief Envoie des données au serveur.
             * @param data Pointeur vers les données à envoyer.
             * @param datalen Longueur des données à envoyer.
             * @return true si l'envoi réussit, sinon false.
             */
            bool send(const unsigned char* data, unsigned int datalen);

            /**
             * @brief Met à jour le gestionnaire d'envoi.
             */
            void update();

            /**
             * @brief Renvoie la taille de la file d'attente d'envoi.
             * @return Taille de la file d'attente.
             */
            size_t queueSize() const;

        private:
            /**
             * @brief Envoie le tampon en attente.
             * @return true si l'envoi réussit, sinon false.
             */
            bool sendPendingBuffer();

            /**
             * @brief Prépare l'en-tête du prochain message à envoyer.
             */
            void prepareNextHeader();

            /**
             * @brief Prépare les données du prochain message à envoyer.
             */
            void prepareNextData();

        private:
            std::list<std::vector<unsigned char>> mQueueingBuffers; /**< File d'attente des tampons à envoyer. */
            std::vector<unsigned char> mSendingBuffer; /**< Tampon actuellement en cours d'envoi. */
            SOCKET mSocket{ INVALID_SOCKET }; /**< Socket associé au gestionnaire d'envoi. */
            State mState{ State::Idle }; /**< État actuel du gestionnaire d'envoi. */
        };


        class ClientImpl
        {
            enum class State {
                Connecting,
                Connected,
                Disconnected,
            };

        public:
            ClientImpl() = default;
            ~ClientImpl();

            /**
             * @brief Initialise la connexion client avec le socket et l'adresse spécifiés.
             * @param sckt Socket à associer au client.
             * @param addr Adresse à associer au client.
             * @return true si l'initialisation réussit, sinon false.
             */
            bool init(SOCKET&& sckt, const sockaddr_in& addr);

            /**
             * @brief Établit une connexion avec le serveur à l'adresse et au port spécifiés.
             * @param ipaddress Adresse IP du serveur.
             * @param port Port du serveur.
             * @return true si la connexion réussit, sinon false.
             */
            bool connect(const std::string& ipaddress, unsigned short port);

            /**
             * @brief Ferme la connexion avec le serveur.
             */
            void disconnect();

            /**
             * @brief Envoie des données au serveur.
             * @param data Pointeur vers les données à envoyer.
             * @param len Longueur des données à envoyer.
             * @return true si l'envoi réussit, sinon false.
             */
            bool send(const unsigned char* data, unsigned int len);

            /**
             * @brief Récupère les messages entrants du serveur.
             * @return Un pointeur unique vers le message reçu.
             */
            std::unique_ptr<Messages::Base> poll();

            /**
             * @brief Récupère l'identifiant du client.
             * @return L'identifiant du client.
             */
            uint64_t id() const { return static_cast<uint64_t>(mSocket); }

            /**
             * @brief Récupère l'adresse de destination du client.
             * @return L'adresse de destination du client.
             */
            const sockaddr_in& destinationAddress() const { return mAddress; }

        private:
            /**
             * @brief Callback appelé lorsqu'une connexion est établie avec succès.
             * @param addr Adresse du serveur connecté.
             */
            void onConnected(const sockaddr_in& addr);

        private:
            ConnectionHandler mConnectionHandler;
            SendingHandler mSendingHandler;
            ReceptionHandler mReceivingHandler;
            sockaddr_in mAddress{ 0 }; /**< Adresse associée au client. */
            SOCKET mSocket{ INVALID_SOCKET }; /**< Socket associé au client. */
            State mState{ State::Disconnected }; /**< État actuel du client. */
        };
    }
}