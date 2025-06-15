#pragma once
#include "Console.h"

const String MAIN = "MAIN_CONSOLE"
const String MARQUEE = "MARQUEE_CONSOLE"
const String SHEDULE = "SCHEDULE_CONSOLE"



class ConsoleManager{
public:
    typedef std::unordered_map<String, std::shared_ptr<Console>> ConsoleTable;

    static ConsoleManager* getInstance();
    static void initialize_console_manager();
    static void destroy_console_manager();
private: 
    ConsoleManager();
    ~ConsoleManager() = default
    ConsoleManager(ConsoleManager const&) {}
    ConsoleManager& operator=(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance;

    ConsoleTable consoleTable
};