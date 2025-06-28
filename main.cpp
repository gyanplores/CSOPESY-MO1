#include "ConsoleManager.h"
// g++ -std=c++20 Console.cpp ConsoleManager.cpp Core.cpp main.cpp MainConsole.cpp Process.cpp SchedulingConsole.cpp Screen.cpp -o main
int main(){
    bool running = true;
    ConsoleManager::initialize_console_manager();
    while(running){
        ConsoleManager::get_instance()->draw_console();
        ConsoleManager::get_instance()->process();

        running = ConsoleManager::get_instance()->running_checker();
    }
    ConsoleManager::destroy_console_manager();
    return 0;
}