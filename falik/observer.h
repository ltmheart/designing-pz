#pragma once
#include <iomanip>
#include <iostream>
#include <vector>
#include "Transaction.h"

// спостерігач
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(const Transaction& t) = 0;
};

// падпіщіки і павідомлення
class TransactionService {
    std::vector<IObserver*> observers;
public:
    void subscribe(IObserver* o) { observers.push_back(o); }
    void notify(const Transaction& t) {
        for (auto* o : observers) o->update(t);
    }
};

class ConsoleLogger : public IObserver {
public:
    void update(const Transaction& t) override {
        std::cout << "  |ConsoleLogger| New transaktsyja #" << t.id << ": " << t.description << std::endl;
    }
};

class EmailNotifier : public IObserver {
public:
    void update(const Transaction& t) override {
        std::cout << "  |EmailNotifier| Lyst pri nvy tranzaktsijy#" << t.id << std::endl;
    }
};

class AnalyticsView : public IObserver {
    int count = 0;
    double total = 0;
public:
    void update(const Transaction& t) override {
        count++;
        total += t.calculated;
        std::cout << "  |Analytics| Vsogo transaktsyj: " << count
                  << ", zagalna syma: " << std::fixed << std::setprecision(2) << total << std::endl;
    }
};
