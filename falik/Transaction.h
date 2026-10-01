#pragma once
#include <string>

// дані однієї транзакції
struct Transaction {
    int id;
    std::string description;
    double rawAmount;        // сума до податку
    double calculated;       // сума після стратегій
    std::string strategyName;
    std::string chain;
    std::string processed;
};
