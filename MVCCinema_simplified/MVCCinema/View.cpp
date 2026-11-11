#include "View.h"
#include <iostream>
#include <limits>

void CinemaView::showMenu() const {
    std::cout << "\nКІНОТЕАТР\n";
    std::cout << "1. Переглянути фільми\n";
    std::cout << "2. Купити квиток\n";
    std::cout << "0. Вийти\n";
}

void CinemaView::showMovies(const std::vector<Movie>& movies) const {
    std::cout << "\nСписок фільмів:\n";
    for (const Movie& movie : movies) {
        std::cout << movie.id << ". " << movie.title
                  << " | Ціна: " << movie.price << " грн"
                  << " | Вільних місць: " << movie.freeSeats << '\n';
    }
}

int CinemaView::readNumber(const std::string& prompt) const {
    int number;
    while (true) {
        std::cout << prompt;
        if (std::cin >> number) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return number;
        }
        if (std::cin.eof()) { // перев. чи завершився потік введення
            return 0;
        }
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Помилка: введіть ціле число!\n";
    }
}

void CinemaView::showMessage(const std::string& message) const {
    std::cout << message << '\n';
}
