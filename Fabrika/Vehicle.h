
#pragma once
#include <string>
using namespace std;

class Vehicle {
protected:
    string type;
    int speed;

public:
    Vehicle(string t, int s);

    void info();

    virtual void drive() = 0;

    virtual ~Vehicle();
};
