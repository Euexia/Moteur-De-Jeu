#pragma once

#include <winSock2.h>

namespace Network
{
    namespace Errors
    {
        /**
         * @brief Récupère le code d'erreur du dernier appel réseau.
         * @return Le code d'erreur du dernier appel réseau.
         */
        int Get();

        /**
         * @brief Enumération des codes d'erreur réseau courants.
         */
        enum {
            AGAIN = WSATRY_AGAIN, /**< Ressayer l'opération. */
            WOULDBLOCK = WSAEWOULDBLOCK, /**< L'opération bloquerait le socket. */
            INPROGRESS = WSAEINPROGRESS, /**< L'opération est en cours. */
            INTR = WSAEINTR, /**< L'appel système a été interrompu avant qu'il ne se termine. */
        };
    }
}