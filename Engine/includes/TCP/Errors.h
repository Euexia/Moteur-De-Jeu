#pragma once

#include <winSock2.h>

namespace Network
{
    namespace Errors
    {
        /**
         * @brief R�cup�re le code d'erreur du dernier appel r�seau.
         * @return Le code d'erreur du dernier appel r�seau.
         */
        int Get();

        /**
         * @brief Enum�ration des codes d'erreur r�seau courants.
         */
        enum {
            AGAIN = WSATRY_AGAIN, /**< Ressayer l'op�ration. */
            WOULDBLOCK = WSAEWOULDBLOCK, /**< L'op�ration bloquerait le socket. */
            INPROGRESS = WSAEINPROGRESS, /**< L'op�ration est en cours. */
            INTR = WSAEINTR, /**< L'appel syst�me a �t� interrompu avant qu'il ne se termine. */
        };
    }
}