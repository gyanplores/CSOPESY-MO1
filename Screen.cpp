#include "Screen.h"
#include "ConsoleManager.h"
#include "SchedulingConsole.h"

#include <iomanip>
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
       ConsoleManager::get_instance()->switch_console(SCHEDULE);
    }
}

void Screen::printProcess() const {
    auto* scheduler = dynamic_cast<SchedulingConsole*>(
    ConsoleManager::get_instance()->getConsoleTable().at("SCHEDULE_CONSOLE").get());


    if (!scheduler) {
        std::cout << "[Screen] Unable to fetch scheduler info.\n";
        return;
    }

    std::lock_guard<std::mutex> lock(scheduler->processMutex);

    std::cout << "-----------------------------\n";
    std::cout << "running processes:\n";

    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::RUNNING) {
            std::cout << "process" << std::setw(2) << std::setfill('0') << proc.id
                      << " (" << ctime(&proc.timestamp)
                      << ") Core: " << proc.current_core << " "
                      << proc.instruction_lines_current << " / "
                      << proc.instruction_lines_max << "\n";
        }
    }

    std::cout << "\nFinished processes:\n";
    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::FINISHED) {
            std::cout << "process" << std::setw(2) << std::setfill('0') << proc.id
                      << " (" << ctime(&proc.timestamp)
                      << ") Finished "
                      << proc.instruction_lines_current << " / "
                      << proc.instruction_lines_max << "\n";
        }
    }
    std::cout << "--------------------------------------------\n";
}
