#pragma once

#include <string>
#include <unordered_map>

class HttpResponse {
public:
    HttpResponse();

    HttpResponse& status(int code);
    HttpResponse& header(const std::string& key, const std::string& value);
    HttpResponse& body(const std::string& content);
    HttpResponse& body(std::string&& content);
    HttpResponse& content_type(const std::string& type);

    static HttpResponse ok();
    static HttpResponse not_found();
    static HttpResponse bad_request();
    static HttpResponse internal_error();
    static HttpResponse method_not_allowed();

    static HttpResponse json(const std::string& json_str);
    static HttpResponse file(const std::string& content, const std::string& mime_type);

    std::string serialize() const;
    int status_code() const { return status_code_; }

private:
    int status_code_;
    std::string status_text_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

    static std::string status_text_for(int code);
};
