#include <iostream>
#include <windows.h>

using namespace std;

// інтерфейс стратегії
class IWeaponStrategy {
public:
    virtual ~IWeaponStrategy() {}
    virtual void useWeapon() = 0;
};

// 4 класи зброі
class PanStrategy : public IWeaponStrategy {
public:
    void useWeapon() override {
        cout << "б'є ворога чавунною сковорідкою з криком: 'СНІДАНОК ГОТОВИЙ!' \n";
    }
};

class MagicWandStrategy : public IWeaponStrategy {
public:
    void useWeapon() override {
        cout << "махає паличкою і закидає ворога гарячими варениками з картоплею!\n";
    }
};

class BowStrategy : public IWeaponStrategy {
public:
    void useWeapon() override {
        cout << "запускає влучну стрілу з прив'язаною до неї сосискою!\n";
    }
};

class SwordStrategy : public IWeaponStrategy {
public:
    void useWeapon() override {
        cout << "розмахує 50-кілограмовим мечем і випадково збиває люстру!\n";
    }
};