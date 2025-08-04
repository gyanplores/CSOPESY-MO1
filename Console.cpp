#include "Console.h"
#include <string>

Console::Console(std::string name): name(name){}

std::string Console::getName() {
    return name;
}
