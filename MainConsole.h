#pragma once
#include <memory>
#include <string>
#include "Console.h"

class MainConsole : public Console{
public: 
    MainConsole();
    void onEnabled() override;
    void display() override;
    void process() override;
private:
    void printCommands() const;
    void tokenize()
};