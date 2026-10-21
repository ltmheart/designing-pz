#include "IngredientDecorator.h"

IngredientDecorator::IngredientDecorator(Pizza* base,
    const std::string& ingredient,
    int extraPrice)
    : base(base), ingredient(ingredient), extraPrice(extraPrice) {}

IngredientDecorator::~IngredientDecorator() {
    delete base;
}

std::string IngredientDecorator::description() const {
    return base->description() + ", " + ingredient;
}

int IngredientDecorator::price() const {
    return base->price() + extraPrice;
}
