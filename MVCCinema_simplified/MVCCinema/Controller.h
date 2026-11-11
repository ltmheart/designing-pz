#pragma once
#include "Model.h"
#include "View.h"

// CONTROLLER: вирішує, що робити після вибору користувача.
class CinemaController {
private:
    CinemaModel model;
    CinemaView view;

public:
    void run();
};
