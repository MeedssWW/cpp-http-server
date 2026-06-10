#pragma once

#include "http_request.h"
#include "http_response.h"
#include "static_file_server.h"
#include <atomic>
#include <chrono>
#include <thread>

class Router {
public:
    explicit Router(const std::string& static_root);

    HttpResponse route(const HttpRequest& request);

    size_t total_requests() const { return total_requests_; }

private:
    StaticFileServer file_server_;
    std::atomic<size_t> total_requests_{0};
    std::chrono::steady_clock::time_point start_time_;

    HttpResponse handle_api_status(const HttpRequest& req);
    HttpResponse handle_api_info(const HttpRequest& req);
};
