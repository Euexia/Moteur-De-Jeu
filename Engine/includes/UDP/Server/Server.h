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
//         * @brief Implémentation privée de la classe Server.
//         */
//        class ServerImplUDP;
//
//        /**
//         * @brief La classe Server représente un serveur UDP.
//         */
//        class ServerUDP
//        {
//        public:
//            /**
//             * @brief Constructeur par défaut de la classe Server.
//             */
//            ServerUDP() {}
//
//            /**
//             * @brief Constructeur de copie supprimé.
//             */
//            ServerUDP(const ServerUDP&) = delete;
//
//            /**
//             * @brief Opérateur d'assignation de copie supprimé.
//             */
//            ServerUDP& operator=(const ServerUDP&) = delete;
//
//            /**
//             * @brief Constructeur de déplacement de la classe Server.
//             */
//            ServerUDP(ServerUDP&&);
//
//            /**
//             * @brief Opérateur d'assignation par déplacement de la classe Server.
//             */
//            ServerUDP& operator=(ServerUDP&&);
//
//            /**
//             * @brief Destructeur de la classe Server.
//             */
//            ~ServerUDP() {}
//
//            /**
//             * @brief Démarre le serveur sur le port spécifié.
//             * @param _port Port sur lequel démarrer le serveur.
//             * @return true si le démarrage réussit, sinon false.
//             */
//            bool start(unsigned short _port);
//
//            /**
//             * @brief Arrête le serveur.
//             */
//            void stop();
//
//            /**
//             * @brief Met à jour le serveur pour traiter les événements en attente.
//             */
//            void update();
//
//            /**
//             * @brief Envoie des données à un client spécifié.
//             * @param clientid Identifiant du client.
//             * @param data Pointeur vers les données à envoyer.
//             * @param len Longueur des données à envoyer.
//             * @return true si l'envoi réussit, sinon false.
//             */
//            bool sendTo(uint64_t clientid, const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Envoie des données à tous les clients connectés.
//             * @param data Pointeur vers les données à envoyer.
//             * @param len Longueur des données à envoyer.
//             * @return true si l'envoi réussit, sinon false.
//             */
//            bool sendToAll(const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Récupère les messages entrants du serveur.
//             * @return Un pointeur unique vers le message reçu.
//             */
//            std::unique_ptr<Messages::Base> poll();
//
//        private:
//            std::unique_ptr<ServerImplUDP> mImpl; /**< Implémentation privée du serveur. */
//        };
//
//        class ServerImplUDP
//        {
//        public:
//            ServerImplUDP() = default;
//            ~ServerImplUDP();
//
//            /**
//             * @brief Démarre le serveur sur le port spécifié.
//             * @param _port Port sur lequel démarrer le serveur.
//             * @return true si le démarrage réussit, sinon false.
//             */
//            bool start(unsigned short _port);
//
//            /**
//             * @brief Arrête le serveur.
//             */
//            void stop();
//
//            /**
//             * @brief Met à jour le serveur.
//             */
//            void update();
//
//            /**
//             * @brief Envoie des données à un client spécifique.
//             * @param clientid Identifiant du client.
//             * @param data Pointeur vers les données à envoyer.
//             * @param len Longueur des données à envoyer.
//             * @return true si l'envoi réussit, sinon false.
//             */
//            bool sendTo(uint64_t clientid, const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Envoie des données à tous les clients connectés.
//             * @param data Pointeur vers les données à envoyer.
//             * @param len Longueur des données à envoyer.
//             * @return true si l'envoi réussit, sinon false.
//             */
//            bool sendToAll(const unsigned char* data, unsigned int len);
//
//            /**
//             * @brief Récupère les messages entrants.
//             * @return Un pointeur unique vers le message reçu.
//             */
//            std::unique_ptr<Messages::Base> poll();
//
//        private:
//            std::map<uint64_t, ClientUDP> mClients; /**< Map des clients connectés, indexés par leur identifiant. */
//            std::list<std::unique_ptr<Messages::Base>> mMessages; /**< Liste des messages entrants. */
//            SOCKET mSocket{ INVALID_SOCKET }; /**< Socket du serveur. */
//        };
//    }
//}