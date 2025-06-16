#pragma once
#include "Console.h"

class SchedulingConsole : public Console {
public:
    SchedulingConsole();
    void onEnabled() override;
    void display() override;
    void process() override;
};
