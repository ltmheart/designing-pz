
#include "VehicleFactory.h"
#include "Car.h"

Vehicle* VehicleFactory::create(int choice) {
    if (choice == 1)
        return new Motorcycle();

    if (choice == 2)
        return new Tractor();

    if (choice == 3)
        return new Taxi();

    return nullptr;
}
