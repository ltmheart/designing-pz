#pragma once
#include <memory>
#include <string>
#include "Observer.h"
#include "Strategy.h"
#include "TransactionModel.h"
#include "View.h"

class Controller {
    TransactionModel& model;
    ConsoleView& view;
    TransactionService& service;
    std::unique_ptr<ITaxCalculationStrategy> strategy;

public:
    Controller(TransactionModel& m, ConsoleView& v, TransactionService& s)
        : model(m), view(v), service(s), strategy(std::make_unique<StandardTaxStrategy>()) {}

    // зміна стратегії
    void setStrategy(std::unique_ptr<ITaxCalculationStrategy> s) {
        strategy = std::move(s);
        view.showMessage("Strategyjy zmineno na: " + strategy->name());
    }

    void addTransaction(const std::string& description, double amount) {
        view.showMessage("dodajemo tranzaktsyjy \"" + description + "\"");

        Transaction t;
        t.id = model.nextId();
        t.description = description;
        t.rawAmount = amount;
        t.calculated = strategy->calculate(amount);
        t.strategyName = strategy->name();
        t.chain = "-";
        t.processed = std::to_string(t.calculated);

        model.add(t);
        view.showTransaction(t);
        service.notify(t);
    }
};
