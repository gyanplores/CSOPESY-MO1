#include "MainConsole.h"
#include "ConsoleManager.h"
#include "SchedulingConsole.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

MainConsole::MainConsole()
  : Console("MAIN_CONSOLE") {}

void MainConsole::onEnabled() {
    std::cout
      << " _______   _______    _______   _______   _______   _______    __    __\n"
      << "/  ___  \\ /   __  \\  /   _   \\ /   _   \\ /   ____| /   __  \\  |  |  |  |\n"
      << "| |   \\_| |  |__|__| |  | |  | |  |_|  | |  |____  |  |__|__| |  \\__|  |\n"
      << "| |    _  \\_____   \\ |  | |  | |   ____/ |   ____| \\_____   \\  \\____   |\n"
      << "| |___/ | |  |__|  | |  |_|  | |  |      |  |____  |  |__|  | |  |__|  |\n"
      << "\\_______/ \\_______/  \\_______/ |__|      \\_______| \\_______/  \\_______/\n\n"
      << "Welcome to CSOPESY commandline!\n"
      << "Type 'exit' to quit, 'clear' to clear screen, and 'help' to view commands\n";
}

void MainConsole::display() {
    std::cout << "Type a command: ";
}

void MainConsole::process() {
    std::string command, setcommand, name;
    std::getline(std::cin, command);
    tokenizeCommand(command, setcommand, name);

    switch (hashString(setcommand)) {
        case StringCode::exit:
            ConsoleManager::get_instance()->exit_application();
            break;
        case StringCode::clear:
            system("cls");
            onEnabled();
            break;
        case StringCode::help:
            std::cout
              << "Available commands:\n"
              << "  exit            - Quit the program\n"
              << "  clear           - Clear the screen\n"
              << "  help            - Show this help message\n"
              << "  initialize      - Run initialization routine\n"
              << "  screen          - Perform screen-related action\n"
              << "  scheduler-test  - Enter scheduler console\n"
              << "  scheduler-stop  - Stop the scheduler (inside scheduler console)\n"
              << "  report-util     - Run report utility\n";
            break;
        case StringCode::scheduler_test:
<<<<<<< Updated upstream
            ConsoleManager::get_instance()->switch_console(SCHEDULE);
=======
            if(this->initialized == 1){
                ConsoleManager::get_instance()->switch_console("SCHEDULING_CONSOLE");
            }else{
                std::cout << "Please use initialized command before proceeding!. \n";
            }
>>>>>>> Stashed changes
            break;
        case StringCode::scheduler_stop:
            // if typed here, no effect; actual stop done inside SchedulingConsole
            std::cout << "Use 'scheduler-stop' inside the scheduler console.\n";
            break;
        case StringCode::report_util:
            std::cout << "report-util command recognized. Doing something.\n";
            break;
        default:
            std::cout << "error: unknown command. Please type a valid command.\n";
    }
}

void MainConsole::tokenizeCommand(const std::string& command, std::string& setcommand, std::string& name) {
    std::istringstream fullstring(command);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(fullstring, token, ' ')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 2) {
        setcommand = tokens[0] + " " + tokens[1];
        name       = tokens[2];
    } else {
        setcommand = tokens[0];
    }
}

MainConsole::StringCode MainConsole::hashString(const std::string& str) {
    if      (str == "exit")           return StringCode::exit;
    else if (str == "clear")          return StringCode::clear;
    else if (str == "help")           return StringCode::help;
    else if (str == "initialize")     return StringCode::initialize;
    else if (str == "screen -r")      return StringCode::screen;
    else if (str == "scheduler-test") return StringCode::scheduler_test;
    else if (str == "scheduler-stop") return StringCode::scheduler_stop;
    else if (str == "report-util")    return StringCode::report_util;
    else                               return StringCode::unknown;
}
