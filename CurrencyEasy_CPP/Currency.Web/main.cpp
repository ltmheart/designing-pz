#include "../Shared/Network.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

// MODEL: дані про валюту (те саме, що ми вчили у MVC).
struct Currency {
    std::string code, name, date;
    double rate = 0;
};

// Отримує дані від ОКРЕМОГО проєкту Currency.Service (порт 8081).
bool loadCurrency(const std::string& code, Currency& currency, std::string& error) {
    Response api = getLocal(8081, "/api/rate?code=" + code);
    if (api.status == 404) { error = "Валюту " + code + " не знайдено."; return false; }
    if (api.status != 200) {
        error = api.body.empty() ? "Currency.Service не запущений."
            : "Не вдалося отримати курс НБУ. Перевірте інтернет.";
        return false;
    }
    currency.code = jsonField(api.body, "code");
    currency.name = jsonField(api.body, "name");
    currency.date = jsonField(api.body, "updatedAt");
    try { currency.rate = std::stod(jsonField(api.body, "rate")); } catch (...) { }
    if (currency.code != code || currency.name.empty() ||
        currency.date.empty() || !std::isfinite(currency.rate) || currency.rate <= 0) {
        error = "Сервіс повернув неправильні дані.";
        return false;
    }
    return true;
}

// VIEW: створює звичайну HTML-сторінку з формою і результатом.
std::string view(const std::string& result) {
    return R"HTML(<!doctype html><html lang="uk"><head><meta charset="utf-8">
<title>Курси валют</title><style>
body{font-family:Arial,sans-serif;background:#f5f6fa;margin:0;color:#222}
main{max-width:550px;background:white;margin:50px auto;padding:30px;
 border-radius:14px;box-shadow:0 4px 18px #0001}
label{display:block;margin:17px 0 6px}input,select,button{box-sizing:border-box;
 width:100%;padding:11px;font-size:16px;border:1px solid #bbb;border-radius:7px}
button{margin-top:23px;background:#4158d8;color:white;border:0;cursor:pointer}
.result{background:#eef2ff;margin-top:25px;padding:15px;border-radius:8px}
.error{background:#ffeded;color:#a00000;margin-top:25px;padding:15px;border-radius:8px}
small{color:#666}
</style></head><body><main><h1>Курси валют</h1>
<p>Пошук за кодом валюти та конвертація за курсом НБУ</p>
<form method="get" action="/">
<label>З якої валюти (USD, EUR, PLN...)</label>
<input name="from" value="USD" maxlength="3" required>
<label>У яку валюту</label>
<select name="to"><option value="UAH">UAH — гривня</option>
<option value="USD">USD — долар</option><option value="EUR">EUR — євро</option>
<option value="PLN">PLN — злотий</option></select>
<label>Сума</label><input name="amount" type="number" step="any" min="0" value="10" required>
<button type="submit">Показати курс і конвертувати</button></form>
)HTML" + result + R"HTML(
<p><small>Джерело: офіційний API Національного банку України.</small></p>
</main></body></html>)HTML";
}

// CONTROLLER: перевіряє введення, звертається до сервісу та передає результат View.
Response controller(const std::string& url) {
    std::string result;
    if (url.find('?') != std::string::npos) {
        std::string from = upperCode(getParameter(url, "from"));
        std::string to = upperCode(getParameter(url, "to"));
        std::string amountText = getParameter(url, "amount");
        double amount = -1;
        try {
            size_t used = 0;
            amount = std::stod(amountText, &used);
            if (used != amountText.size() || !std::isfinite(amount)) amount = -1;
        } catch (...) { }

        std::string error;
        if (from.empty() || to.empty()) error = "Введіть коди валют з трьох латинських літер.";
        else if (amount < 0 || amount > 1000000000) error = "Введіть правильну додатну суму або нуль.";
        else {
            Currency first, second;
            if (loadCurrency(from, first, error) && loadCurrency(to, second, error)) {
                // Через гривню: 1 USD = rate(USD) UAH, 1 EUR = rate(EUR) UAH.
                double converted = amount * first.rate / second.rate;
                std::ostringstream out;
                out << std::fixed << std::setprecision(4);
                out << "<div class='result'><b>" << htmlEscape(first.name) << " (" << from << ")</b>"
                    << "<p>Курс: 1 " << from << " = " << first.rate << " грн</p>"
                    << "<p>Дата курсу: " << htmlEscape(first.date) << "</p>";
                if (to != "UAH")
                    out << "<p>1 " << to << " = " << second.rate
                        << " грн (" << htmlEscape(second.date) << ")</p>";
                out << std::setprecision(2) << "<h2>" << amount << " " << from
                    << " = " << converted << " " << to << "</h2></div>";
                result = out.str();
            }
        }
        if (!error.empty()) result = "<div class='error'>" + htmlEscape(error) + "</div>";
    }
    return { 200, "text/html; charset=utf-8", view(result) };
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Network network;
    if (!network.ready()) return 1;
    std::cout << "Currency.Web: відкрийте http://127.0.0.1:8080/\n";
    if (!runServer(8080, controller)) std::cout << "Не вдалося відкрити порт 8080\n";
    return 0;
}
