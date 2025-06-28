#pragma once
#include "Screen.h"

class ProcessScreen : public Screen {
public:
    ProcessScreen();

    void setProcessName(const std::string& name);

    void onEnabled() override;
    void display() override;
    void process() override;

private:
    std::string selectedProcess;
};
