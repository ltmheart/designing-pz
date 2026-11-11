#pragma once
#include <string>
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")

// Допоміжна функція: отримати справжню відповідь НБУ по HTTPS.
// Налаштовувати ключ не потрібно: це відкриті дані НБУ.
inline std::string askNbu(const std::string& code) {
    HINTERNET session = WinHttpOpen(L"CurrencyEasy/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, nullptr, nullptr, 0);
    if (!session) return "";

    HINTERNET connect = WinHttpConnect(session, L"bank.gov.ua", 443, 0);
    if (!connect) {
        WinHttpCloseHandle(session);
        return "";
    }

    // API приймає код із трьох латинських літер: USD, EUR, PLN ...
    std::wstring path = L"/NBUStatService/v1/statdirectory/exchange?json&valcode=";
    for (char letter : code) path += static_cast<wchar_t>(letter);

    HINTERNET request = WinHttpOpenRequest(connect, L"GET", path.c_str(),
        nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    std::string result;
    if (request) {
        WinHttpSetTimeouts(request, 5000, 5000, 5000, 5000);
        if (WinHttpSendRequest(request, nullptr, 0, nullptr, 0, 0, 0) &&
            WinHttpReceiveResponse(request, nullptr)) {
            DWORD status = 0;
            DWORD size = sizeof(status);
            bool success = WinHttpQueryHeaders(request,
                WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX) && status == 200;
            if (success) {
                char buffer[4096];
                DWORD bytes = 0;
                while (WinHttpReadData(request, buffer, sizeof(buffer), &bytes) && bytes > 0) {
                    result.append(buffer, bytes);
                    if (result.size() > 200000) { result.clear(); break; }
                }
            }
        }
        WinHttpCloseHandle(request);
    }
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return result; // порожній рядок: НБУ не відповів
}
