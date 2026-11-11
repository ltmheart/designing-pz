
#include <windows.h>
#include "Controller.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    CinemaController app;
    app.run();

    return 0;
}
