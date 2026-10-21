#pragma once
#include "Pizza.h"

class IngredientDecorator : public Pizza {
private:
    Pizza* base;
    std::string ingredient;
    int extraPrice;

public:
    IngredientDecorator(Pizza* base, const std::string& ingredient,
        int extraPrice);
    ~IngredientDecorator() override;
    std::string description() const override;
    int price() const override;
};
