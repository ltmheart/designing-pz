#include "Model.h"

CinemaModel::CinemaModel() {
    movies = {
        {1, "Інтерстеллар", 250, 5},
        {2, "Аватар", 220, 3},
        {3, "Дюна", 240, 0}
    };
}

const std::vector<Movie>& CinemaModel::getMovies() const {
    return movies;
}

bool CinemaModel::buyTicket(int movieId) {
    for (Movie& movie : movies) {
        if (movie.id == movieId && movie.freeSeats > 0) {
            movie.freeSeats--;
            return true;
        }
    }
    return false;
}
