#pragma once
#include <string>

class Console{
public:
    Console(std::string name);
    ~Console() = default;

    std::string getName();
    virtual void onEnabled() = 0;
    virtual void display() = 0;
    virtual void process() = 0;

    std::string name;
    friend class ConsoleManager;
};