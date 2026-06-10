#include "server.h"
#include "http_request.h"
#include "http_response.h"
#include "logger.h"
#include <csignal>
#include <chrono>

Server* Server::instance_ = nullptr;

Server::Server(uint16_t port, size_t num_threads, const std::string& static_root)
    : winsock_()
    , port_(port)
    , num_threads_(num_threads)
    , listener_()
    , pool_(num_threads)
    , router_(static_root)
    , running_(false) {
    instance_ = this;
}

Server::~Server() {
    stop();
    instance_ = nullptr;
}

void Server::start() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    listener_.set_reuse_addr(true);

    if (!listener_.bind(port_)) return;
    if (!listener_.listen(128)) return;

    running_ = true;

    Logger::info("========================================");
    Logger::info("  MiniHTTP Server v1.0");
    Logger::info("  Port:    " + std::to_string(port_));
    Logger::info("  Threads: " + std::to_string(num_threads_));
    Logger::info("  URL:     http://localhost:" + std::to_string(port_) + "/");
    Logger::info("  Press Ctrl+C to stop");
    Logger::info("========================================");

    while (running_) {
        std::string client_ip;
        TcpSocket client = listener_.accept(client_ip);

        if (!client.is_valid()) {
            if (running_) Logger::debug("Accept returned invalid socket");
            continue;
        }

        auto client_ptr = std::make_shared<TcpSocket>(std::move(client));

        pool_.enqueue([this, client_ptr, client_ip]() {
            handle_client(*client_ptr, client_ip);
        });
    }

    Logger::info("Server stopped. Total requests: "
                 + std::to_string(router_.total_requests()));
}

void Server::stop() {
    if (running_.exchange(false)) {
        listener_.close();
        Logger::info("Shutting down gracefully...");
    }
}

void Server::handle_client(TcpSocket& client, const std::string& ) {
    auto start = std::chrono::steady_clock::now();

    client.set_timeout(5);

    std::string raw = client.receive();
    if (raw.empty()) return;

    HttpRequest request = HttpRequest::parse(raw);

    HttpResponse response;
    if (!request.is_valid()) {
        response = HttpResponse::bad_request();
    } else {
        response = router_.route(request);
    }

    client.send(response.serialize());

    auto end = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    Logger::request(
        request.is_valid() ? request.method() : "???",
        request.is_valid() ? request.path() : "???",
        response.status_code(),
        ms
    );
}

void Server::signal_handler(int sig) {
    Logger::info("Received signal " + std::to_string(sig));
    if (instance_) instance_->stop();
}
