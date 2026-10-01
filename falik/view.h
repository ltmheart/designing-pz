#pragma once
#include <iostream>
#include <string>
#include "Transaction.h"

// вью
class ConsoleView {
public:
    void showMessage(const std::string& msg) {
        std::cout << "\n>>> " << msg << std::endl;
    }
    void showTransaction(const Transaction& t) {
        std::cout << "  Opys: " << t.description << std::endl;
        std::cout << "  Syma: " << t.rawAmount << std::endl;
        std::cout << "  Strategy: " << t.strategyName << " -> " << t.calculated << std::endl;
        std::cout << "  Dekoratory: " << t.chain << std::endl;
        std::cout << "  Rezultaty: " << t.processed << std::endl;
    }
};
