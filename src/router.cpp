#include "router.h"
#include <sstream>

Router::Router(const std::string& static_root)
    : file_server_(static_root)
    , start_time_(std::chrono::steady_clock::now()) {}

HttpResponse Router::route(const HttpRequest& request) {
    ++total_requests_;

    const std::string& path = request.path();

    if (path == "/api/status") return handle_api_status(request);
    if (path == "/api/info")   return handle_api_info(request);

    return file_server_.serve(path);
}

HttpResponse Router::handle_api_status(const HttpRequest& ) {
    auto uptime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - start_time_
    ).count();

    std::ostringstream json;
    json << "{\n"
         << "  \"status\": \"running\",\n"
         << "  \"uptime_seconds\": " << uptime << ",\n"
         << "  \"total_requests\": " << total_requests_.load() << ",\n"
         << "  \"thread_id\": \"" << std::this_thread::get_id() << "\"\n"
         << "}";

    return HttpResponse::json(json.str());
}

HttpResponse Router::handle_api_info(const HttpRequest& ) {
    std::ostringstream json;
    json << "{\n"
         << "  \"server\": \"MiniHTTP/1.0\",\n"
         << "  \"language\": \"C++17\",\n"
         << "  \"platform\": \""
#ifdef _WIN32
         << "Windows"
#elif __linux__
         << "Linux"
#elif __APPLE__
         << "macOS"
#else
         << "Unknown"
#endif
         << "\",\n"
         << "  \"features\": [\"thread-pool\", \"static-files\", \"json-api\"]\n"
         << "}";

    return HttpResponse::json(json.str());
}
