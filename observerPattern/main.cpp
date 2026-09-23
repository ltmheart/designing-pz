#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

// інтерфейс для всіх пристроїв
class Observer {
public:
    // метод оновлення, який викликає хаб
    virtual void update(int temp) = 0;
};

// центральний хаб (суб'єкт)
class Hub {
private:
    // список підключених пристроїв
    vector<Observer*> devices;

public:
    // підключення нового пристрою
    void add(Observer* dev) {
        devices.push_back(dev);
        cout << "пристрій додано\n";
    }

    // відключення пристрою за вказівником
    void remove(Observer* dev) {
        for (size_t i = 0; i < devices.size(); i++) {
            if (devices[i] == dev) {
                devices.erase(devices.begin() + i);
                cout << "пристрій видалено\n";
                break;
            }
        }
    }

    // зміна температури та сповіщення всіх пристроїв
    void setTemp(int temp) {
        cout << "\nнові дані: " << temp << "°c\n";
        for (Observer* dev : devices) {
            dev->update(temp); // викликаємо update для кожного пристрою
        }
    }
};
