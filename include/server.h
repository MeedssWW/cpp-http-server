#pragma once

#include "platform.h"
#include "socket.h"
#include "thread_pool.h"
#include "router.h"
#include <atomic>
#include <string>
#include <memory>

class Server {
public:
    Server(uint16_t port, size_t num_threads, const std::string& static_root);
    ~Server();

    void start();
    void stop();

private:
    void handle_client(TcpSocket& client, const std::string& client_ip);

    WinsockInit winsock_;
    uint16_t port_;
    size_t num_threads_;
    TcpSocket listener_;
    ThreadPool pool_;
    Router router_;
    std::atomic<bool> running_;

    static Server* instance_;
    static void signal_handler(int sig);
};
