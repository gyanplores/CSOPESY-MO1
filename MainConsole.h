#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Console.h"

class MainConsole : public Console{
public: 
    MainConsole();
    void onEnabled() override;
    void display() override;
    void process() override;
    bool initialized = 0;
private:
    enum class StringCode {
        exit,
        clear,
        help,
        initialize,
        screen,
        process,
        scheduler_test,
        scheduler_stop,
        report_util,
        unknown
    };
    void tokenizeCommand(const std::string& command, std::string& setcommand, std::string& name);
    StringCode hashString(const std::string& str);

};