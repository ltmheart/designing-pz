#pragma once
#include "Vehicle.h"

class VehicleFactory {
public:
    static Vehicle* create(int choice);
};
