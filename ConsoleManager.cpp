#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"

#include <iostream>
#include <Windows.h>
#include <string>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::get_instance() {
    return sharedInstance;
}

void ConsoleManager::initialize_console_manager() {
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy_console_manager() {
    delete sharedInstance;
    sharedInstance = nullptr;
}

void ConsoleManager::draw_console() const {
    curr_console->display();
}

void ConsoleManager::process() const {
    curr_console->process();
}

void ConsoleManager::switch_console(std::string console_name) {
    system("cls");
    prev_console = curr_console;
    curr_console = console_table[console_name];
    curr_console->onEnabled();
}

void ConsoleManager::return_console() {
    std::swap(curr_console, prev_console);
    system("cls");
    curr_console->onEnabled();
}

bool ConsoleManager::running_checker() const {
    return running;
}

void ConsoleManager::exit_application() {
    running = false;
}

ConsoleManager::ConsoleManager() {
    running = true;
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Register consoles
    auto main_console       = std::make_shared<MainConsole>();
    // auto marquee_console  = std::make_shared<MarqueeConsole>();
    auto scheduling_console = std::make_shared<SchedulingConsole>();
    // auto memory_console   = std::make_shared<MemoryConsole>();

<<<<<<< Updated upstream
    console_table[MAIN]     = main_console;
    // console_table[MARQUEE] = marquee_console;
    console_table[SCHEDULE] = scheduling_console;
    // console_table[MEMORY]  = memory_console;
=======
    this->console_table[MAIN] = main_console;
    this->console_table[SCHEDULE] = scheduling_console;
    this->console_table["SCHEDULING_CONSOLE"] = scheduling_console;
    this->console_table["SCREEN_VIEW"] = screen_console;
>>>>>>> Stashed changes

    switch_console(MAIN);
}

HANDLE ConsoleManager::getConsoleHandle() const {
    return console_handle;
}
