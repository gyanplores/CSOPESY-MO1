#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"
#include "Screen.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <string>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::get_instance(){
    return sharedInstance;
}

void ConsoleManager::initialize_console_manager(){
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy_console_manager(){
    delete sharedInstance;
    sharedInstance = nullptr;
}

void ConsoleManager::initialize_console(){
    std::ifstream file("config.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "num-cpu") {
            int value;
            iss >> value;
            CORE::N_CORE = value;
            std::cout << "[Config] Set number of cores to " << CORE::N_CORE << "\n";
        }
        if (key == "delay-per-exec"){
            int value;
            iss >> value;
            CORE::DELAY = value;
            std::cout << "[Config] set delay to " << CORE::DELAY << "\n";
        }
    }

    file.close();
}

void ConsoleManager::draw_console() const {
    this->curr_console->display();
}

void ConsoleManager::process() const{
    this->curr_console->process();
}

void ConsoleManager::switch_console(std::string console_name){
    system("cls");
    this->prev_console = this->curr_console;
    this->curr_console = this->console_table[console_name];
    this->curr_console->onEnabled();
}

void ConsoleManager::return_console(){
    std::shared_ptr<Console> temp = this->curr_console;
    this->curr_console = this->prev_console;
    this->prev_console = temp;

    system("cls");
    this->curr_console->onEnabled();
}

bool ConsoleManager::running_checker() const {
    return running;
}

void ConsoleManager::exit_application(){
    running = false;
}

ConsoleManager::ConsoleManager(){
    this->running = true;
    this->console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    const std::shared_ptr<MainConsole> main_console = std::make_shared<MainConsole>();
    const std::shared_ptr<SchedulingConsole> scheduling_console = std::make_shared<SchedulingConsole>();
    std::shared_ptr<Screen> screen_console = std::make_shared<Screen>("SCREEN_VIEW");

    this->console_table[MAIN] = main_console;
    this->console_table[SCHEDULE] = scheduling_console;
    this->console_table["SCHEDULING_CONSOLE"] = scheduling_console;
    this->console_table["SCREEN_VIEW"] = screen_console;

    this->switch_console(MAIN);
}

HANDLE ConsoleManager::getConsoleHandle() const{
    return console_handle;
}

ConsoleManager::ConsoleTable& ConsoleManager::getConsoleTable() {
    return this->console_table;
}
