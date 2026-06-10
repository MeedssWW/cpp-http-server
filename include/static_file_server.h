#pragma once

#include "http_response.h"
#include <string>
#include <filesystem>

class StaticFileServer {
public:
    explicit StaticFileServer(const std::string& root_dir);

    HttpResponse serve(const std::string& request_path);

private:
    std::filesystem::path root_;

    static std::string get_mime_type(const std::string& extension);
    bool is_safe_path(const std::filesystem::path& path) const;
    std::string read_file(const std::filesystem::path& path);
};
