#include "Controller.h"

void CinemaController::run() {
    int choice;

    do {
        view.showMenu();
        choice = view.readNumber("Ваш вибір: ");

        switch (choice) {
        case 1:
            view.showMovies(model.getMovies());
            break;

        case 2: {
            view.showMovies(model.getMovies());
            int id = view.readNumber("Введіть номер фільму: ");

            if (model.buyTicket(id)) {
                view.showMessage("Квиток успішно куплено!");
            } else {
                view.showMessage("Помилка: фільму немає або квитки закінчилися.");
            }
            break;
        }

        case 0:
            view.showMessage("До побачення!");
            break;

        default:
            view.showMessage("Помилка: такого пункту меню немає.");
            break;
        }
    } while (choice != 0);
}
