#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"

#include <iostream>
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
}

void ConsoleManager::draw_console(){
    this->curr_console->display();
}

void ConsoleManager::process(){
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

bool ConsoleManager::running_checker(){
    return running;
}

void ConsoleManager::exit_application(){
    running = false;
}

ConsoleManager::ConsoleManager(){
    this->running = true;
    //this->console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    const std::shared_ptr<MainConsole> main_console = std::make_shared<MainConsole>();
    const std::shared_ptr<MarqueeConsole> marquee_console = std::make_shared<MarqueeConsole>();
    const std::shared_ptr<SchedulingConsole> scheduling_console = std::make_shared<SchedulingConsole>();
    const std::shared_ptr<MemoryConsole> memory_console = std::make_shared<MemoryConsole>();

    this->console_table[MAIN_CONSOLE] = main_console;
    this->console_table[MARQUEE_CONSOLE] = marquee_console;
    this->console_table[SCHEDULE_CONSOLE] = scheduling_console;
    this->console_table[MEMORY_CONSOLE] = memory_console;

    this->switch_console(MAIN_CONSOLE);
}