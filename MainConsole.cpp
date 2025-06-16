#include "MainConsole.h"
#include "ConsoleManager.h"

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
}

void MainConsole::display(){
    std::cout << "Welcome to CSOPESY commandline!" << "\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear screen, and 'help' to view commands" << "\n";
    std::cout << "Type a command: ";
}

void MainConsole::process(){
    std::string command, setcommand, name;

    std::getline(std::cin, command); // Takes commands with spaces into consideration

    tokenizeCommand(command, setcommand, name); //  Tokenize command
    
    // std::cout << "Command is: " << setcommand << " Name is:" << name << "\n\n"; //  For Testing

    switch (hashString(setcommand)) {
        case StringCode::exit:
            ConsoleManager::get_instance()->exit_application();  // Exit program
            break;
        case StringCode::clear:
            system("cls");  // Clear screen (Windows-specific)
            onEnabled(); // Reprint Header
            break;
        case StringCode::help:
            // Display available commands
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
            std::cout << "initialize command recognized. Doing something.\n";
            break;
        case StringCode::screen:
            std::cout << "screed command recognized. Doing something.\n";
            break;
        case StringCode::scheduler_test:
            std::cout << "scheduler-test command recognized. Doing something.\n";
            break;
        case StringCode::scheduler_stop:
            std::cout << "scheduler-stop command recognized. Doing something.\n";
            break;
        case StringCode::report_util:
            std::cout << "report-util command recognized. Doing something.\n";
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
        setcommand = tokens[0] + " " + tokens[1];   //  Specifically made for "screen -r"
        name = tokens[2];
    }else{
        setcommand = tokens[0];
    }
}

MainConsole::StringCode MainConsole::hashString(const std::string& str) {
    if (str == "exit") return StringCode::exit;
    if (str == "clear") return StringCode::clear;
    if (str == "help") return StringCode::help;
    if (str == "initialize") return StringCode::initialize;
    if (str == "screen -r") return StringCode::screen;
    if (str == "scheduler-test") return StringCode::scheduler_test;
    if (str == "scheduler-stop") return StringCode::scheduler_stop;
    if (str == "report-util") return StringCode::report_util;
    return StringCode::unknown;  // Return unknown if command doesn't match any known ones
}