
#include <iostream>
#include <windows.h>
#include "VehicleFactory.h"
using namespace std;

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    int choice;

    cout << "Оберіть транспорт:" << endl;
    cout << "1 - Мотоцикл" << endl;
    cout << "2 - Трактор" << endl;
    cout << "3 - Таксі" << endl;

    cout << "Ваш вибір: ";

    if (!(cin >> choice)) {
        cout << "Помилка! Введіть число." << endl;
        return 0;
    }

    Vehicle* vehicle = VehicleFactory::create(choice);

    if (vehicle == nullptr) {
        cout << "Помилка! Такого транспорту немає." << endl;
        return 0;
    }

    vehicle->info();
    vehicle->drive();

    delete vehicle;

    return 0;
}
