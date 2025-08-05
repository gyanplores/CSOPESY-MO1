#include "ConsoleManager.h"
#include "MemoryManager.h"

MemoryManager memoryManager(16384, 4096);

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