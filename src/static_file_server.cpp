#include "static_file_server.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

StaticFileServer::StaticFileServer(const std::string& root_dir)
    : root_(std::filesystem::absolute(root_dir)) {
    if (!std::filesystem::exists(root_)) {
        std::filesystem::create_directories(root_);
        Logger::warn("Created missing document root: " + root_.string());
    }
    Logger::info("Static files root: " + root_.string());
}

HttpResponse StaticFileServer::serve(const std::string& request_path) {
    std::string path = request_path;

    if (path == "/" || path.empty()) {
        path = "/index.html";
    }

    std::filesystem::path full_path = root_ / path.substr(1);
    full_path = std::filesystem::weakly_canonical(full_path);

    if (!is_safe_path(full_path)) {
        Logger::warn("Path traversal blocked: " + request_path);
        return HttpResponse::not_found();
    }

    if (std::filesystem::is_directory(full_path)) {
        full_path /= "index.html";
    }

    if (!std::filesystem::exists(full_path)) {
        return HttpResponse::not_found();
    }

    std::string content = read_file(full_path);
    if (content.empty() && std::filesystem::file_size(full_path) > 0) {
        return HttpResponse::internal_error();
    }

    std::string ext = full_path.extension().string();
    std::string mime = get_mime_type(ext);

    return HttpResponse::file(content, mime);
}

bool StaticFileServer::is_safe_path(const std::filesystem::path& path) const {
    std::string root_str = root_.string();
    std::string path_str = path.string();
    return path_str.compare(0, root_str.size(), root_str) == 0;
}

std::string StaticFileServer::read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return "";

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

std::string StaticFileServer::get_mime_type(const std::string& extension) {
    static const std::unordered_map<std::string, std::string> mime_types = {
        {".html", "text/html; charset=utf-8"},
        {".htm",  "text/html; charset=utf-8"},
        {".css",  "text/css; charset=utf-8"},
        {".js",   "application/javascript; charset=utf-8"},
        {".json", "application/json; charset=utf-8"},
        {".txt",  "text/plain; charset=utf-8"},
        {".xml",  "application/xml; charset=utf-8"},
        {".png",  "image/png"},
        {".jpg",  "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif",  "image/gif"},
        {".ico",  "image/x-icon"},
        {".svg",  "image/svg+xml"},
        {".webp", "image/webp"},
        {".woff", "font/woff"},
        {".woff2","font/woff2"},
        {".ttf",  "font/ttf"},
        {".pdf",  "application/pdf"},
    };

    auto it = mime_types.find(extension);
    return (it != mime_types.end()) ? it->second : "application/octet-stream";
}
