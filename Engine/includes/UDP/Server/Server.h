//#pragma once
//
//#include "UDP/Client/Client.h"
//
//#include "TCP/Sockets.h"
//#include "TCP/Messages.h"
//
//#include <memory>
//#include <map>
//#include <list>
//#include <cassert>
//
//namespace Network
//{
//    namespace Messages {
//        class Base;
//    }
//    namespace UDP
//    {
//        /**
//         * @brief Impl�mentation priv�e de la classe Server.
//         */
//        class ServerImplUDP;
//
//        /**
//         * @brief La classe Server repr�sente un serveur UDP.
//         */
//        class ServerUDP
//        {
//        public:
//            /**
//             * @brief Constructeur par d�faut de la classe Server.
//             */
//            ServerUDP() {}
//
//            /**
//             * @brief Constructeur de copie supprim�.
//             */
//            ServerUDP(const ServerUDP&) = delete;
//
//            /**
//             * @brief Op�rateur d'assignation de copie supprim�.
//             */
//            ServerUDP& operator=(const ServerUDP&) = delete;
//
//            /**
//             * @brief Constructeur de d�placement de la classe Server.
//             */
//            ServerUDP(ServerUDP&&);
//
//            /**
//             * @brief Op�rateur d'assignation par d�placement de la classe Server.
//             */
//            ServerUDP& operator=(ServerUDP&&);
//
//            /**
//             * @brief Destructeur de la classe Server.
//             */
//            ~ServerUDP() {}
//
//            /**
//             * @brief D�marre le serveur sur le port sp�cifi�.
//             * @param _port Port sur lequel d�marrer le serveur.
//             * @return true si le d�marrage r�ussit, sinon false.
//             */
//            bool start(unsigned short _port);
//
//            /**
//             * @brief Arr�te le serveur.
//             */
//            void stop();
//
//            /**
//             * @brief Met � jour le serveur pour traiter les �v�nements en attente.
//             */
//            void update();
//
//            /**
//             * @brief Envoie des donn�es � un client sp�cifi�.
//             * @param clientid Identifiant du client.
//             * @param data Pointeur vers les donn�es � envoyer.
//             * @param len Longueur des donn�es � envoyer.
//             * @return true si l'envoi r�ussit, sinon false.
//             */
//            bool sendTo(uint64_t clientid, const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Envoie des donn�es � tous les clients connect�s.
//             * @param data Pointeur vers les donn�es � envoyer.
//             * @param len Longueur des donn�es � envoyer.
//             * @return true si l'envoi r�ussit, sinon false.
//             */
//            bool sendToAll(const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief R�cup�re les messages entrants du serveur.
//             * @return Un pointeur unique vers le message re�u.
//             */
//            std::unique_ptr<Messages::Base> poll();
//
//        private:
//            std::unique_ptr<ServerImplUDP> mImpl; /**< Impl�mentation priv�e du serveur. */
//        };
//
//        class ServerImplUDP
//        {
//        public:
//            ServerImplUDP() = default;
//            ~ServerImplUDP();
//
//            /**
//             * @brief D�marre le serveur sur le port sp�cifi�.
//             * @param _port Port sur lequel d�marrer le serveur.
//             * @return true si le d�marrage r�ussit, sinon false.
//             */
//            bool start(unsigned short _port);
//
//            /**
//             * @brief Arr�te le serveur.
//             */
//            void stop();
//
//            /**
//             * @brief Met � jour le serveur.
//             */
//            void update();
//
//            /**
//             * @brief Envoie des donn�es � un client sp�cifique.
//             * @param clientid Identifiant du client.
//             * @param data Pointeur vers les donn�es � envoyer.
//             * @param len Longueur des donn�es � envoyer.
//             * @return true si l'envoi r�ussit, sinon false.
//             */
//            bool sendTo(uint64_t clientid, const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Envoie des donn�es � tous les clients connect�s.
//             * @param data Pointeur vers les donn�es � envoyer.
//             * @param len Longueur des donn�es � envoyer.
//             * @return true si l'envoi r�ussit, sinon false.
//             */
//            bool sendToAll(const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief R�cup�re les messages entrants.
//             * @return Un pointeur unique vers le message re�u.
//             */
//            std::unique_ptr<Messages::Base> poll();
//
//        private:
//            std::map<uint64_t, ClientUDP> mClients; /**< Map des clients connect�s, index�s par leur identifiant. */
//            std::list<std::unique_ptr<Messages::Base>> mMessages; /**< Liste des messages entrants. */
//            SOCKET mSocket{ INVALID_SOCKET }; /**< Socket du serveur. */
//        };
//    }
//}