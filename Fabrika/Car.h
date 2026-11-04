
#pragma once
#include "Vehicle.h"

class Motorcycle : public Vehicle {
public:
    Motorcycle();
    void drive() override;
};

class Tractor : public Vehicle {
public:
    Tractor();
    void drive() override;
};

class Taxi : public Vehicle {
public:
    Taxi();
    void drive() override;
};
#pragma once
