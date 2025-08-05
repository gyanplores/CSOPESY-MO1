#include "Var.h"
#include <iostream>
#include <vector>
#include <string>

using String = std::string;

Var::Var(String name, int value, int vAddr) : name(name), value(value), virtualAddress(vAddr) {}