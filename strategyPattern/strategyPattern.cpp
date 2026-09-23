#include <iostream>
#include <windows.h>

using namespace std;

// інтерфейс стратегії для зброї
class IWeaponStrategy {
public:
    virtual ~IWeaponStrategy() {}
    virtual void useWeapon() = 0; // метод використання зброї
};

// конкретні стратегії (різні види зброї)
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

// контекст: базовий клас персонажа, який тримає стратегію
class Character {
protected:
    IWeaponStrategy* weaponStrategy = nullptr; // поточна стратегія

public:
    virtual ~Character() {
        delete weaponStrategy; // очищення пам'яті
    }

    // зміна стратегії (зброї) на льоту
    void setWeapon(IWeaponStrategy* newWeapon) {
        delete weaponStrategy;
        weaponStrategy = newWeapon;
        cout << "-> [ЗБРОЮ ЗМІНЕНО НА ЛЬОТУ!]\n";
    }

    // виклик методу поточної стратегії
    void fight() {
        if (weaponStrategy) {
            weaponStrategy->useWeapon();
        }
        else {
            cout << "стоїть і панічно махає руками...\n";
        }
    }

    virtual void display() = 0;
};

// конкретні персонажі з початковою зброєю
class Cook : public Character {
public:
    Cook() { weaponStrategy = new PanStrategy(); }
    void display() override { cout << "\n-- бойовий шеф-кухар (дуже лютий) --\n"; }
};

class Fairy : public Character {
public:
    Fairy() { weaponStrategy = new MagicWandStrategy(); }
    void display() override { cout << "\n-- маленька фея (ростом 15 см) --\n"; }
};

class Elf : public Character {
public:
    Elf() { weaponStrategy = new BowStrategy(); }
    void display() override { cout << "\n-- лісовий ельф --\n"; }
};

class Knight : public Character {
public:
    Knight() { weaponStrategy = new SwordStrategy(); }
    void display() override { cout << "\n-- закований у броню лицар --\n"; }
};

int main() {
    // консолічка
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // створення персонажа кухаря та тестування зміни зброї
    Character* cook = new Cook();
    cook->display();
    cook->fight();

    cout << "\n[кухар віджав у феї чарівну паличку, бо подумав, що це кулінарний вінчик]\n";
    cook->setWeapon(new MagicWandStrategy());
    cook->fight();

    // створення феї
    Character* fairy = new Fairy();
    fairy->display();
    fairy->fight();

    cout << "\n[фея забрала в лицаря меч, але він заважкий, тому вона падає під його вагою]\n";
    fairy->setWeapon(new SwordStrategy());
    fairy->fight();

    // створення лицаря
    Character* knight = new Knight();
    knight->display();

    cout << "\n[лицар викинув свій меч і забрав у кухаря сковорідку, бо вона завдає більше ушкоджень]\n";
    knight->setWeapon(new PanStrategy());
    knight->fight();

    // очищення пам'яті
    delete cook;
    delete fairy;
    delete knight;

    return 0;
}
