#include "ConsoleManager.h"
#include "MainConsole.h"
#include "SchedulingConsole.h"

#include <iostream>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::get_instance(){
    return sharedInstance;
}
