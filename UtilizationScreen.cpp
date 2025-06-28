#include "UtilizationScreen.h"
#include "SchedulingConsole.h"
#include "ConsoleManager.h"
#include "ProcessScreen.h"
#include <iostream>
#include <iomanip>
#include <mutex>

UtilizationScreen::UtilizationScreen() : Screen("UTIL_SCREEN") {}

void UtilizationScreen::onEnabled() {
    std::cout << "[Screen] Viewing CPU Utilization screen.\n";
}

void UtilizationScreen::display() {
    system("cls");
    printProcess();  // from base class
}

void UtilizationScreen::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "exit") {
        ConsoleManager::get_instance()->switch_console(MAIN);
    } else if (cmd == "report -util") {
        generateFile();  // from base class
    } 
}
