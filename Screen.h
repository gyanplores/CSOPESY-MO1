#pragma once

#include "Console.h"

class Screen : public Console {
public:
    Screen(std::string processName);

    void onEnabled() override;
    void display() override;
    void process() override;

private:
    void printProcess(std::string processName);
    void printProcess() const;
    void generateFile() const;
};
