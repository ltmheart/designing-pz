#pragma once

// Мережеві функції для локальних HTTP-запитів.
// Порядок важливий: winsock2.h підключаємо перед windows.h.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <functional>

#pragma comment(lib, "Ws2_32.lib")

struct Response {
    int status = 200;
    std::string type = "text/plain; charset=utf-8";
    std::string body;
};

// Ініціалізація мережі Windows.
class Network {
public:
    Network() { ok = WSAStartup(MAKEWORD(2, 2), &data) == 0; }
    ~Network() { if (ok) WSACleanup(); }
    bool ready() const { return ok; }
private:
    WSADATA data{};
    bool ok = false;
};

inline std::string statusText(int code) {
    if (code == 200) return "OK";
    if (code == 400) return "Bad Request";
    if (code == 404) return "Not Found";
    if (code == 502) return "Bad Gateway";
    return "Internal Server Error";
}

inline bool sendAll(SOCKET s, const std::string& text) {
    size_t pos = 0;
    while (pos < text.size()) {
        int sent = send(s, text.data() + pos, static_cast<int>(text.size() - pos), 0);
        if (sent <= 0) return false;
        pos += sent;
    }
    return true;
}

// Запускає локальний HTTP-сервер. Кожен запит обробляємо по черзі.
inline bool runServer(unsigned short port, const std::function<Response(const std::string&)>& handle) {
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET) return false;

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // лише цей комп'ютер

    if (bind(listener, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR ||
        listen(listener, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(listener);
        return false;
    }

    while (true) {
        SOCKET client = accept(listener, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;
        DWORD timeout = 10000;
        setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));
        std::string request;
        char buffer[2048];
        while (request.find("\r\n\r\n") == std::string::npos && request.size() < 8192) {
            int count = recv(client, buffer, sizeof(buffer), 0);
            if (count <= 0) break;
            request.append(buffer, count);
        }

        Response reply{ 400, "text/plain; charset=utf-8", "Неправильний запит" };
        if (request.rfind("GET ", 0) == 0) {
            size_t end = request.find(' ', 4);
            if (end != std::string::npos) {
                reply = handle(request.substr(4, end - 4));
            }
        }
        std::string header = "HTTP/1.1 " + std::to_string(reply.status) + " " + statusText(reply.status) + "\r\n"
            "Content-Type: " + reply.type + "\r\n"
            "Content-Length: " + std::to_string(reply.body.size()) + "\r\n"
            "Connection: close\r\n\r\n";
        sendAll(client, header + reply.body);
        shutdown(client, SD_BOTH);
        closesocket(client);
    }
    // Сервер працює, доки не закрити вікно консолі.
    return true;
}

// HTTP-клієнт для звернення до іншого локального C++ сервісу.
inline Response getLocal(unsigned short port, const std::string& path) {
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return { 502, "", "" };
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    DWORD timeout = 7000;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));
    if (connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        closesocket(s);
        return { 502, "", "" };
    }
    std::string request = "GET " + path + " HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    if (!sendAll(s, request)) { closesocket(s); return { 502, "", "" }; }
    std::string result;
    char buffer[4096];
    int n = 0;
    while ((n = recv(s, buffer, sizeof(buffer), 0)) > 0 && result.size() < 1024 * 1024) {
        result.append(buffer, n);
    }
    closesocket(s);
    auto split = result.find("\r\n\r\n");
    if (split == std::string::npos) return { 502, "", "" };
    int status = 502;
    if (result.rfind("HTTP/1.1 ", 0) == 0) {
        try { status = std::stoi(result.substr(9, 3)); } catch (...) {}
    }
    return { status, "application/json; charset=utf-8", result.substr(split + 4) };
}

inline std::string getParameter(const std::string& url, const std::string& name) {
    size_t q = url.find('?');
    if (q == std::string::npos) return "";
    size_t start = q + 1;
    while (start < url.size()) {
        size_t end = url.find('&', start);
        if (end == std::string::npos) end = url.size();
        size_t eq = url.find('=', start);
        if (eq != std::string::npos && eq < end && url.substr(start, eq - start) == name)
            return url.substr(eq + 1, end - eq - 1);
        start = end + 1;
    }
    return "";
}

inline std::string upperCode(std::string code) {
    if (code.size() != 3) return "";
    for (char& c : code) {
        if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
        if (c < 'A' || c > 'Z') return "";
    }
    return code;
}

// Прості допоміжні функції для полів JSON із API НБУ.
// Невеликий помічник для читання JSON. Може читати і українські Unicode-escape.
inline std::string utf8Codepoint(unsigned int c) {
    std::string s;
    if (c <= 0x7F) s += static_cast<char>(c);
    else if (c <= 0x7FF) {
        s += static_cast<char>(0xC0 | (c >> 6));
        s += static_cast<char>(0x80 | (c & 63));
    } else if (c <= 0xFFFF) {
        s += static_cast<char>(0xE0 | (c >> 12));
        s += static_cast<char>(0x80 | ((c >> 6) & 63));
        s += static_cast<char>(0x80 | (c & 63));
    } else if (c <= 0x10FFFF) {
        s += static_cast<char>(0xF0 | (c >> 18));
        s += static_cast<char>(0x80 | ((c >> 12) & 63));
        s += static_cast<char>(0x80 | ((c >> 6) & 63));
        s += static_cast<char>(0x80 | (c & 63));
    }
    return s;
}
inline int hexDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}
inline std::string jsonField(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\n')) ++pos;
    if (pos == json.size()) return "";
    if (json[pos] == '"') {
        ++pos;
        std::string value;
        while (pos < json.size() && json[pos] != '"') {
            if (json[pos] != '\\') { value += json[pos++]; continue; }
            if (++pos >= json.size()) return "";
            char escape = json[pos++];
            if (escape == '"' || escape == '\\' || escape == '/') value += escape;
            else if (escape == 'n') value += '\n';
            else if (escape == 't') value += '\t';
            else if (escape == 'u' && pos + 4 <= json.size()) {
                unsigned int cp = 0;
                for (int i = 0; i < 4; ++i) {
                    int d = hexDigit(json[pos++]);
                    if (d < 0) return "";
                    cp = cp * 16 + static_cast<unsigned int>(d);
                }
                value += utf8Codepoint(cp);
            } else return "";
        }
        return value;
    }
    size_t end = json.find_first_of(",}\r\n", pos);
    return json.substr(pos, end == std::string::npos ? end : end - pos);
}

inline std::string jsonEscape(const std::string& value) {
    std::string out;
    for (char c : value) {
        if (c == '"' || c == '\\') out += '\\';
        if (c == '\n' || c == '\r' || c == '\t') continue;
        out += c;
    }
    return out;
}

inline std::string htmlEscape(const std::string& text) {
    std::string out;
    for (char c : text) {
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else if (c == '\'') out += "&#39;";
        else out += c;
    }
    return out;
}
