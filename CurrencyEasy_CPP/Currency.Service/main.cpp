#include "../Shared/Network.h"
#include "../Shared/Nbu.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

// СЕРВІС: отримує справжній курс і повертає JSON.
Response getRate(const std::string& url) {
    if (url != "/api/rate" && url.rfind("/api/rate?", 0) != 0)
        return { 404, "application/json; charset=utf-8", "{\"error\":\"Адресу не знайдено\"}" };

    std::string code = upperCode(getParameter(url, "code"));
    if (code.empty())
        return { 400, "application/json; charset=utf-8", "{\"error\":\"Введіть код валюти\"}" };

    // Для гривні курс до самої себе дорівнює 1.
    if (code == "UAH")
        return { 200, "application/json; charset=utf-8",
            "{\"code\":\"UAH\",\"name\":\"Українська гривня\",\"rate\":1,\"updatedAt\":\"—\"}" };

    std::string data = askNbu(code);
    if (data.empty())
        return { 502, "application/json; charset=utf-8", "{\"error\":\"НБУ недоступний\"}" };

    // НБУ повертає [] для невідомої валюти.
    if (data.find("\"rate\"") == std::string::npos) {
        if (data.find('[') != std::string::npos)
            return { 404, "application/json; charset=utf-8", "{\"error\":\"Валюту не знайдено\"}" };
        return { 502, "application/json; charset=utf-8", "{\"error\":\"Неправильна відповідь НБУ\"}" };
    }

    std::string name = jsonField(data, "txt");
    std::string rateText = jsonField(data, "rate");
    std::string date = jsonField(data, "exchangedate");
    double rate = 0;
    try { rate = std::stod(rateText); } catch (...) { }
    if (name.empty() || date.empty() || !std::isfinite(rate) || rate <= 0)
        return { 502, "application/json; charset=utf-8", "{\"error\":\"Неправильні дані НБУ\"}" };

    std::ostringstream json;
    json << std::fixed << std::setprecision(6);
    json << "{\"code\":\"" << code << "\",\"name\":\"" << jsonEscape(name)
         << "\",\"rate\":" << rate << ",\"updatedAt\":\"" << jsonEscape(date) << "\"}";
    return { 200, "application/json; charset=utf-8", json.str() };
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Network network;
    if (!network.ready()) return 1;
    std::cout << "Currency.Service: http://127.0.0.1:8081/api/rate?code=USD\n";
    if (!runServer(8081, getRate)) std::cout << "Не вдалося відкрити порт 8081\n";
    return 0;
}
