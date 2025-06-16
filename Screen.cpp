#include "Screen.h"
#include "ConsoleManager.h"
#include <iostream>

Screen::Screen(std::string processName) : Console(processName) {}

void Screen::onEnabled() {
    std::cout << "[Screen] Viewing process output screen.\n";
}

void Screen::display() {
    printProcess();
}

void Screen::process() {
    std::string cmd;
    std::getline(std::cin, cmd);
    if (cmd == "exit") {
        ConsoleManager::get_instance()->return_console();
    }
}

void Screen::printProcess() const {
    std::cout << "[Screen] Simulating output display...\n";
}
