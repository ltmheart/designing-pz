
#include "Car.h"
#include <iostream>
using namespace std;

// Мотоцикл
Motorcycle::Motorcycle() : Vehicle("Мотоцикл", 200) {}

void Motorcycle::drive() {
    cout << "Мотоцикл швидко їде дорогою." << endl;
}

// Трактор
Tractor::Tractor() : Vehicle("Трактор", 40) {}

void Tractor::drive() {
    cout << "Трактор працює в полі." << endl;
}

// Таксі
Taxi::Taxi() : Vehicle("Таксі", 160) {}

void Taxi::drive() {
    cout << "Таксі перевозить пасажирів містом." << endl;
}
