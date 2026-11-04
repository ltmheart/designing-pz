
#include "Vehicle.h"
#include <iostream>
using namespace std;

Vehicle::Vehicle(string t, int s) {
    type = t;
    speed = s;
}

void Vehicle::info() {
    cout << "Транспорт: " << type << endl;
    cout << "Максимальна швидкість: " << speed << " км/год" << endl;
}

Vehicle::~Vehicle() {}
