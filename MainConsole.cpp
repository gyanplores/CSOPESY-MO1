#include "MainConsole.h"
#include "ConsoleManager.h"
#include "SchedulingConsole.h"
#include "ProcessScreen.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

MainConsole::MainConsole() : Console("MAIN_CONSOLE"){}

void MainConsole::onEnabled(){
    std::cout << " _______   _______    _______   _______   _______   _______    __    __\n" 
              << "/  ___  \\ /   __  \\  /   _   \\ /   _   \\ /   ____| /   __  \\  |  |  |  |\n"
              << "| |   \\_| |  |__|__| |  | |  | |  |_|  | |  |____  |  |__|__| |  \\__|  |\n"
              << "| |    _  \\_____   \\ |  | |  | |   ____/ |   ____| \\_____   \\  \\____   |\n"
              << "| |___/ | |  |__|  | |  |_|  | |  |      |  |____  |  |__|  | |  |__|  |\n"
              << "\\_______/ \\_______/  \\_______/ |__|      \\_______| \\_______/  \\_______/\n\n";
    std::cout << "Welcome to CSOPESY commandline!" << "\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear screen, and 'help' to view commands" << "\n";
}

void MainConsole::display(){
    std::cout << "Type a command: ";
}

void MainConsole::process(){
    std::string command, setcommand, name;
    std::getline(std::cin, command); // Takes commands with spaces into consideration

    if (command.rfind("screen -s", 0) == 0 && command.length() > 10) {
        if(this->initialized == 1){
            std::string process_name = command.substr(10); // Extract process name

            // Switch to ProcessScreen
            ConsoleManager::get_instance()->switch_console("PROC");

            // Pass process name to ProcessScreen
            auto proc_screen = std::dynamic_pointer_cast<ProcessScreen>(
                ConsoleManager::get_instance()->getConsoleTable().at("PROC")
            );
            if (proc_screen) {
                proc_screen->setProcessName(process_name);
            } else {
                std::cout << "Could not access ProcessScreen.\n";
            }

            return; // Exit early
        } else {
            std::cout << "Please use initialize command first..\n"; 
        }
    }

    tokenizeCommand(command, setcommand, name); // Tokenize command

    switch (hashString(setcommand)) {
        case StringCode::exit:
            ConsoleManager::get_instance()->exit_application();
            break;
        case StringCode::clear:
            system("cls");
            onEnabled();
            break;
        case StringCode::help:
            std::cout << "Available commands:\n"
                      << "  exit            - Quit the program\n"
                      << "  clear           - Clear the screen\n"
                      << "  help            - Show this help message\n"
                      << "  initialize      - Run initialization routine\n"
                      << "  screen          - Perform screen-related action\n"
                      << "  scheduler-test  - Test the scheduler\n"
                      << "  scheduler-stop  - Stop the scheduler\n"
                      << "  report-util     - Run report utility\n";
            break;
        case StringCode::initialize:
            ConsoleManager::get_instance()->initialize_console();
            this->initialized = 1;
            break;
        case StringCode::screen:
            if(this->initialized == 1){
                ConsoleManager::get_instance()->switch_console("UTIL");
            } else {
                std::cout << "Please use initialize command first..\n"; 
            }
            break;
        case StringCode::scheduler_test:
            if(this->initialized == 1){
                ConsoleManager::get_instance()->switch_console("SCHEDULING_CONSOLE");
            } else {
                std::cout << "Please use initialized command before proceeding!\n";
            }
            break;
        case StringCode::scheduler_stop:
            if (this->initialized == 1) {
                auto sched_console = std::dynamic_pointer_cast<SchedulingConsole>(
                    ConsoleManager::get_instance()->getConsoleTable().at("SCHEDULING_CONSOLE")
                );
                if (sched_console) {
                    sched_console->stopScheduler();
                    std::cout << "[Main] Sent stop request to scheduler.\n";
                } else {
                    std::cout << "Error: Could not access SchedulingConsole.\n";
                }
            } else {
                std::cout << "Please use initialize command first..\n"; 
            }
            break;
        case StringCode::report_util:
            if(this->initialized == 1){
                ConsoleManager::get_instance()->switch_console("UTIL");
            } else {
                std::cout << "Please use initialize command first..\n"; 
            }
            break;
        case StringCode::unknown:
            std::cout << "error: unknown command. Please type a valid command.\n";
            break;
    }
}

void MainConsole::tokenizeCommand(const std::string& command, std::string& setcommand, std::string& name){
    std::istringstream fullstring(command);
    std::vector<std::string> tokens;
    std::string token;

    while(getline(fullstring, token, ' ')){
        tokens.push_back(token);
    }

    if(tokens.size() >= 2){
        setcommand = tokens[0] + " " + tokens[1];
        name = tokens[2];
    } else {
        setcommand = tokens[0];
    }
}

MainConsole::StringCode MainConsole::hashString(const std::string& str) {
    if (str == "exit") return StringCode::exit;
    if (str == "clear") return StringCode::clear;
    if (str == "help") return StringCode::help;
    if (str == "initialize") return StringCode::initialize;
    if (str == "screen-ls") return StringCode::screen;
    if (str == "scheduler-test") return StringCode::scheduler_test;
    if (str == "scheduler-stop") return StringCode::scheduler_stop;
    if (str == "report-util") return StringCode::report_util;
    return StringCode::unknown;
}
