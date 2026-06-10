#include "http_request.h"
#include <sstream>
#include <algorithm>

HttpRequest HttpRequest::parse(const std::string& raw) {
    HttpRequest req;

    if (raw.empty()) return req;

    std::istringstream stream(raw);
    std::string line;

    if (!std::getline(stream, line)) return req;

    if (!line.empty() && line.back() == '\r') line.pop_back();

    std::istringstream request_line(line);
    if (!(request_line >> req.method_ >> req.path_ >> req.version_)) return req;

    if (req.method_ != "GET") {
        req.valid_ = false;
        return req;
    }

    auto qpos = req.path_.find('?');
    if (qpos != std::string::npos) {
        req.query_string_ = req.path_.substr(qpos + 1);
        req.path_ = req.path_.substr(0, qpos);
    }

    req.path_ = url_decode(req.path_);

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break;

        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);

            auto start = value.find_first_not_of(" \t");
            if (start != std::string::npos) value = value.substr(start);

            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            req.headers_[key] = value;
        }
    }

    req.valid_ = true;
    return req;
}

std::string HttpRequest::header(const std::string& key) const {
    std::string lower_key = key;
    std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), ::tolower);
    auto it = headers_.find(lower_key);
    return (it != headers_.end()) ? it->second : "";
}

std::string HttpRequest::url_decode(const std::string& str) {
    std::string result;
    result.reserve(str.size());

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size()) {
            int hex = 0;
            std::istringstream iss(str.substr(i + 1, 2));
            if (iss >> std::hex >> hex) {
                result += static_cast<char>(hex);
                i += 2;
            } else {
                result += str[i];
            }
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}
