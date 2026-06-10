#include "socket.h"
#include "logger.h"
#include <cstring>

TcpSocket::TcpSocket() : fd_(INVALID_SOCK) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == INVALID_SOCK) {
        Logger::error("Failed to create socket: " + std::to_string(sock_error()));
    }
}

TcpSocket::TcpSocket(socket_t fd) : fd_(fd) {}

TcpSocket::~TcpSocket() {
    close();
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = INVALID_SOCK;
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        other.fd_ = INVALID_SOCK;
    }
    return *this;
}

bool TcpSocket::bind(uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        Logger::error("Bind failed on port " + std::to_string(port)
                      + ": error " + std::to_string(sock_error()));
        return false;
    }
    return true;
}

bool TcpSocket::listen(int backlog) {
    if (::listen(fd_, backlog) < 0) {
        Logger::error("Listen failed: error " + std::to_string(sock_error()));
        return false;
    }
    return true;
}

TcpSocket TcpSocket::accept(std::string& client_ip) {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    socket_t client_fd = ::accept(fd_,
        reinterpret_cast<sockaddr*>(&client_addr), &client_len);

    if (client_fd == INVALID_SOCK) {
        client_ip = "";
        return TcpSocket(INVALID_SOCK);
    }

    char ip_buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_buf, INET_ADDRSTRLEN);
    client_ip = ip_buf;

    return TcpSocket(client_fd);
}

std::string TcpSocket::receive(size_t buffer_size) {
    std::string buffer(buffer_size, '\0');
    int bytes = ::recv(fd_, &buffer[0], static_cast<int>(buffer_size), 0);
    if (bytes <= 0) return "";
    buffer.resize(bytes);
    return buffer;
}

bool TcpSocket::send(const std::string& data) {
    return send(data.c_str(), data.size());
}

bool TcpSocket::send(const char* data, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        int sent = ::send(fd_, data + total_sent,
                          static_cast<int>(length - total_sent), 0);
        if (sent <= 0) return false;
        total_sent += sent;
    }
    return true;
}

void TcpSocket::close() {
    if (fd_ != INVALID_SOCK) {
        sock_close(fd_);
        fd_ = INVALID_SOCK;
    }
}

bool TcpSocket::is_valid() const {
    return fd_ != INVALID_SOCK;
}

void TcpSocket::set_reuse_addr(bool enable) {
    int opt = enable ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&opt), sizeof(opt));
}

void TcpSocket::set_timeout(int seconds) {
#ifdef _WIN32
    DWORD timeout = seconds * 1000;
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO,
               reinterpret_cast<const char*>(&timeout), sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif
}
