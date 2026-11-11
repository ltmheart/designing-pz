#pragma once
#include <string>
#include <vector>

// Один фільм у кінотеатрі.
struct Movie {
    int id;
    std::string title;
    int price;
    int freeSeats;
};

// MODEL: зберігає дані та змінює кількість квитків.
class CinemaModel {
private:
    std::vector<Movie> movies;

public:
    CinemaModel();
    const std::vector<Movie>& getMovies() const;
    bool buyTicket(int movieId);
};
