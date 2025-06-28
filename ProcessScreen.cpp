#include "ProcessScreen.h"
#include "SchedulingConsole.h"
#include "ConsoleManager.h"
#include "ProcessScreen.h"

#include <iostream>

ProcessScreen::ProcessScreen() : Screen("PROCESS_SCREEN") {}

void ProcessScreen::setProcessName(const std::string& name) {
    selectedProcess = name;
}

void ProcessScreen::onEnabled() {
    std::cout << "[Screen] Viewing individual process: " << selectedProcess << "\n";
}

void ProcessScreen::display() {
    system("cls");
    printProcess(selectedProcess);  // from base class
}

void ProcessScreen::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "exit") {
        ConsoleManager::get_instance()->switch_console(MAIN);  // go back to UTIL_SCREEN
    }
}
