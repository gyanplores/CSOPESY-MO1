#pragma once
#include <memory>
#include <string>
#include "Console.h"

class Screen : public Console{
public: 
    Screen(std::string processName);
    void onEnabled() override;
    void display() override;
    void process() override;
private:
    void printProcess() const;
    //std::shared_ptr<Process> attached_process;
};