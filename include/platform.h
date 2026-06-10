#pragma once

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>

    using socket_t = SOCKET;
    constexpr socket_t INVALID_SOCK = INVALID_SOCKET;

    inline int sock_close(socket_t s) { return closesocket(s); }
    inline int sock_error() { return WSAGetLastError(); }

    struct WinsockInit {
        WinsockInit() {
            WSADATA data;
            WSAStartup(MAKEWORD(2, 2), &data);
        }
        ~WinsockInit() { WSACleanup(); }
    };

#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cerrno>

    using socket_t = int;
    constexpr socket_t INVALID_SOCK = -1;

    inline int sock_close(socket_t s) { return close(s); }
    inline int sock_error() { return errno; }

    struct WinsockInit {};
#endif
