#include "Controller.h"
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    TransactionModel model;
    ConsoleView view;
    TransactionService service;

    ConsoleLogger logger;
    EmailNotifier email;
    AnalyticsView analytics;
    service.subscribe(&logger);
    service.subscribe(&email);
    service.subscribe(&analytics);

    Controller controller(model, view, service);

    controller.addTransaction("Oplata poslyg", 1000);

    // зміна стратег.
    controller.setStrategy(std::make_unique<ITSectorTaxStrategy>());
    controller.addTransaction("", 1000);

    return 0;
}
