#pragma once
#include "Model.h"
#include <string>

// VIEW: показує текст і читає введення користувача.
class CinemaView {
public:
    void showMenu() const;
    void showMovies(const std::vector<Movie>& movies) const;
    int readNumber(const std::string& prompt) const;
    void showMessage(const std::string& message) const;
};
