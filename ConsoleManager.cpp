#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"

#include <iostream>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance(){
    return sharedInstance;
}

class ConsoleManager{
    public:
    
}