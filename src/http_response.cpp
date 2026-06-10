#include "http_response.h"
#include <sstream>

HttpResponse::HttpResponse() : status_code_(200), status_text_("OK") {
    headers_["Server"] = "MiniHTTP/1.0";
    headers_["Connection"] = "close";
}

HttpResponse& HttpResponse::status(int code) {
    status_code_ = code;
    status_text_ = status_text_for(code);
    return *this;
}

HttpResponse& HttpResponse::header(const std::string& key, const std::string& value) {
    headers_[key] = value;
    return *this;
}

HttpResponse& HttpResponse::body(const std::string& content) {
    body_ = content;
    headers_["Content-Length"] = std::to_string(body_.size());
    return *this;
}

HttpResponse& HttpResponse::body(std::string&& content) {
    body_ = std::move(content);
    headers_["Content-Length"] = std::to_string(body_.size());
    return *this;
}

HttpResponse& HttpResponse::content_type(const std::string& type) {
    headers_["Content-Type"] = type;
    return *this;
}

HttpResponse HttpResponse::ok() {
    return HttpResponse().status(200);
}

HttpResponse HttpResponse::not_found() {
    return HttpResponse()
        .status(404)
        .content_type("text/html; charset=utf-8")
        .body("<html><body>"
              "<h1>404 Not Found</h1>"
              "<p>The requested resource was not found on this server.</p>"
              "</body></html>");
}

HttpResponse HttpResponse::bad_request() {
    return HttpResponse()
        .status(400)
        .content_type("text/html; charset=utf-8")
        .body("<html><body><h1>400 Bad Request</h1></body></html>");
}

HttpResponse HttpResponse::internal_error() {
    return HttpResponse()
        .status(500)
        .content_type("text/html; charset=utf-8")
        .body("<html><body><h1>500 Internal Server Error</h1></body></html>");
}

HttpResponse HttpResponse::method_not_allowed() {
    return HttpResponse()
        .status(405)
        .content_type("text/html; charset=utf-8")
        .header("Allow", "GET")
        .body("<html><body><h1>405 Method Not Allowed</h1></body></html>");
}

HttpResponse HttpResponse::json(const std::string& json_str) {
    return HttpResponse()
        .status(200)
        .content_type("application/json; charset=utf-8")
        .body(json_str);
}

HttpResponse HttpResponse::file(const std::string& content, const std::string& mime_type) {
    return HttpResponse()
        .status(200)
        .content_type(mime_type)
        .body(content);
}

std::string HttpResponse::serialize() const {
    std::ostringstream oss;

    oss << "HTTP/1.1 " << status_code_ << " " << status_text_ << "\r\n";

    for (const auto& [key, value] : headers_) {
        oss << key << ": " << value << "\r\n";
    }

    oss << "\r\n";

    oss << body_;

    return oss.str();
}

std::string HttpResponse::status_text_for(int code) {
    switch (code) {
        case 200: return "OK";
        case 301: return "Moved Permanently";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default:  return "Unknown";
    }
}
