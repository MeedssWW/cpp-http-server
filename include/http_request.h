#pragma once

#include <string>
#include <unordered_map>

class HttpRequest {
public:
    static HttpRequest parse(const std::string& raw);

    const std::string& method() const { return method_; }
    const std::string& path() const { return path_; }
    const std::string& version() const { return version_; }
    const std::string& query_string() const { return query_string_; }

    std::string header(const std::string& key) const;
    const std::unordered_map<std::string, std::string>& headers() const { return headers_; }

    bool is_valid() const { return valid_; }

private:
    std::string method_;
    std::string path_;
    std::string query_string_;
    std::string version_;
    std::unordered_map<std::string, std::string> headers_;
    bool valid_ = false;

    static std::string url_decode(const std::string& str);
};
