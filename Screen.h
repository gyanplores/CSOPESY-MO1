#pragma once

#include "Console.h"

class Screen : public Console {
public:
    Screen(std::string processName);

    void onEnabled() override;
    void display() override;
    void process() override;
    void generateFile() const;
    void showProcessSMI() const;
    void showVMStat() const;

protected:
    void printProcess(std::string processName);
    void printProcess() const;
};
