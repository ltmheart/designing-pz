#pragma once
#include <string>

class Pizza {
public:
    virtual ~Pizza() = default;
    virtual std::string description() const = 0;
    virtual int price() const = 0;
};
