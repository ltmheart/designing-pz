#pragma once
#include <string>

// різні способи обчислення податку
class ITaxCalculationStrategy {
public:
    virtual ~ITaxCalculationStrategy() = default;
    virtual double calculate(double amount) = 0;
    virtual std::string name() = 0;
};

class StandardTaxStrategy : public ITaxCalculationStrategy {
public:
    double calculate(double a) override { return a * 1.18; }
    std::string name() override { return "Standard tax (18%)"; }
};

class ITSectorTaxStrategy : public ITaxCalculationStrategy {
public:
    double calculate(double a) override { return a * 1.05; }
    std::string name() override { return "IT sector tax (5%)"; }
};
