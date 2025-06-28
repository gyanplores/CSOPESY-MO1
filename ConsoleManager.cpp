#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"
#include "Screen.h"
#include "UtilizationScreen.h"
#include "ProcessScreen.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <string>


ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::get_instance(){
    return sharedInstance;
}

void ConsoleManager::initialize_console_manager(){  //  Instantiates a new console Manager
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy_console_manager(){     //  Destroys the console Manager
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

//void ConsoleManager::register_screen(std::shared_ptr<Screen> screen_ref){}
//void ConsoleManager::switch_screen(std::string screen_name){}
//void ConsoleManager::unregister_screen(std::string screen_name){}

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
    //const std::shared_ptr<MarqueeConsole> marquee_console = std::make_shared<MarqueeConsole>();
    const std::shared_ptr<SchedulingConsole> scheduling_console = std::make_shared<SchedulingConsole>();
    //const std::shared_ptr<MemoryConsole> memory_console = std::make_shared<MemoryConsole>();
    std::shared_ptr<Screen> util_screen = std::make_shared<UtilizationScreen>();
    std::shared_ptr<Screen> proc_screen = std::make_shared<ProcessScreen>();

    this->console_table[MAIN] = main_console;
    //this->console_table[MARQUEE] = marquee_console;
    this->console_table[SCHEDULE] = scheduling_console;
    //this->console_table[MEMORY] = memory_console;
    this->console_table["UTIL"] = util_screen;
    this->console_table["PROC"] = proc_screen;

    this->switch_console(MAIN);
}

HANDLE ConsoleManager::getConsoleHandle() const{
    return console_handle;
}

ConsoleManager::ConsoleTable& ConsoleManager::getConsoleTable() {
    return this->console_table;
}

void ConsoleManager::register_screen(std::shared_ptr<Screen> screen) {
    screen_table[screen->getName()] = screen;
}

void ConsoleManager::unregister_screen(const String& screen_name) {
    screen_table.erase(screen_name);
}

void ConsoleManager::switch_screen(const String& screen_name) {
    if (screen_table.count(screen_name)) {
        prev_screen = curr_screen;
        curr_screen = screen_table[screen_name];
        system("cls");
        curr_screen->onEnabled();
    } else {
        std::cout << "[ConsoleManager] Screen '" << screen_name << "' not found.\n";
    }
}

void ConsoleManager::return_screen() {
    std::swap(curr_screen, prev_screen);
    if (curr_screen) {
        system("cls");
        curr_screen->onEnabled();
    }
}

std::shared_ptr<Screen> ConsoleManager::getCurrentScreen() const {
    return curr_screen;
}

ConsoleManager::ScreenTable& ConsoleManager::getScreenTable() {
    return screen_table;
}
