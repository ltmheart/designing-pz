#include "BasePizza.h"
#include "IngredientDecorator.h"
#include <Windows.h>
#include <iostream>

struct MenuItem {
    const char* name;
    int price;
};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    const MenuItem menu[] = {
        { "сир", 25 },
        { "ковбаса", 35 },
        { "овочі", 20 },
        { "соус", 10 }
    };

    Pizza* order = new BasePizza();
    std::cout << "Замовлення піци\n"
        << "1 - сир, 2 - ковбаса, 3 - овочі, 4 - соус\n"
        << "0 - завершити\n";

    while (true) {
        int choice;
        std::cout << "Оберіть добавку: ";
        if (!(std::cin >> choice)) {
            std::cout << "Помилка: введіть число.\n";
            delete order;
            return 1;
        }
        if (choice == 0) break;
        if (choice < 1 || choice > 4) {
            std::cout << "Такої добавки немає.\n";
            continue;
        }

        const MenuItem& item = menu[choice - 1];
        // додаємо новий шар до піци
        order = new IngredientDecorator(order, item.name, item.price);
        std::cout << "Додано: " << item.name
            << ". Поточна ціна: " << order->price() << " грн\n";
    }

    std::cout << "\nВаша піца: " << order->description()
        << "\nДо сплати: " << order->price() << " грн\n";
    delete order;
    return 0;
}
