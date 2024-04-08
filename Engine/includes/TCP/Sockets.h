#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <winSock2.h>
#if !defined(_MSC_VER)
#include <WS2tcpip.h>
#define inet_ntop(FAMILY, PTR_STRUCT_SOCKADDR, BUFFER, BUFFER_LENGTH) strncpy(BUFFER, inet_ntoa(*static_cast<struct in_addr*>((PTR_STRUCT_SOCKADDR))), BUFFER_LENGTH)
#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
#elif _MSC_VER >= 1800
#include <WS2tcpip.h>
#else
#define inet_ntop(FAMILY, PTR_STRUCT_SOCKADDR, BUFFER, BUFFER_LENGTH) strncpy(BUFFER, inet_ntoa(*static_cast<struct in_addr*>((PTR_STRUCT_SOCKADDR))), BUFFER_LENGTH)
#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
using socklen_t = int;
#endif
using nfds_t = unsigned long;
inline int poll(pollfd fdarray[], nfds_t nfds, int timeout) { return WSAPoll(fdarray, nfds, timeout); }
#else
#include <cerrno> // errno
#define SOCKET int
#define INVALID_SOCKET ((int)-1)
#define SOCKET_ERROR (int(-1))
#endif

#include <string>

namespace Network
{
    /**
     * @brief Initialise les sockets.
     * @return true si l'initialisation r�ussit, sinon false.
     */
    bool Start();

    /**
     * @brief Lib�re les ressources associ�es aux sockets.
     */
    void Release();

    /**
     * @brief Configure un socket en mode non bloquant.
     * @param socket Socket � configurer.
     * @return true si la configuration r�ussit, sinon false.
     */
    bool SetNonBlocking(SOCKET socket);

    /**
     * @brief Configure un socket pour r�utiliser l'adresse.
     * @param socket Socket � configurer.
     * @return true si la configuration r�ussit, sinon false.
     */
    bool SetReuseAddr(SOCKET socket);

    /**
     * @brief Ferme un socket.
     * @param socket Socket � fermer.
     */
    void CloseSocket(SOCKET socket);

    /**
     * @brief R�cup�re l'adresse IP d'une structure sockaddr_in.
     * @param addr Structure sockaddr_in contenant l'adresse.
     * @return L'adresse IP sous forme de cha�ne de caract�res.
     */
    std::string GetAddress(const sockaddr_in& addr);

    /**
     * @brief R�cup�re le port d'une structure sockaddr_in.
     * @param addr Structure sockaddr_in contenant le port.
     * @return Le port sous forme d'entier non sign�.
     */
    unsigned short GetPort(const sockaddr_in& addr);
}