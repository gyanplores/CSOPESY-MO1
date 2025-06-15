#include "ConsoleManager.h"

int main(){
    bool running = true;
    ConsoleManager::initialize_console_manager();
    while(running){
        ConsoleManager::get_instance()->process();
        ConsoleManager::get_instance()->draw_console();

        running = ConsoleManager::get_instance()->running_checker();
    }
    ConsoleManager::destroy_console_manager();
    return 0;
}