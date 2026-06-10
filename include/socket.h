#pragma once

#include "platform.h"
#include <string>
#include <cstdint>

class TcpSocket {
public:
    TcpSocket();
    explicit TcpSocket(socket_t fd);
    ~TcpSocket();

    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;

    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;

    bool bind(uint16_t port);
    bool listen(int backlog = 128);
    TcpSocket accept(std::string& client_ip);

    std::string receive(size_t buffer_size = 8192);
    bool send(const std::string& data);
    bool send(const char* data, size_t length);

    void close();
    bool is_valid() const;
    socket_t raw() const { return fd_; }

    void set_reuse_addr(bool enable);
    void set_timeout(int seconds);

private:
    socket_t fd_;
};
