#pragma once
#include "Pizza.h"

class BasePizza : public Pizza {
public:
    std::string description() const override;
    int price() const override;
};
