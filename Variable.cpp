#include "Variable.h"
#include <iostream>
#include <vector>
#include <string>

using String = std::string;

Var::Var(String name, int value) : name(name), value(value) {};
Var::Var(String name, int value, int memory) : name(name), value(value), memory(memory) {};