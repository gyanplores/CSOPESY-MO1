#pragma once

#include <string>
#include <vector>

using String = std::string;

class SymbolTable {

public:
    String name;
    int value;
    bool inUse = 0;
    int memory;

public:
    Var(String name, int value);
};
