#pragma once

#include <string>

using String = std::string;

class Var {

public:
    String name;
    int value;
    bool inUse = 0;

public:
    Var(String name, int value);
};
