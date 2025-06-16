#pragma once
#include "Console.h"
#include "Screen.h"
#include "SchedulingConsole.h"
#include <unordered_map>
#include <Windows.h>
#include <string>
#include <memory>

using String = std::string;

const String MAIN     = "MAIN_CONSOLE";
const String MARQUEE  = "MARQUEE_CONSOLE";
const String SCHEDULE = "SCHEDULE_CONSOLE";

class ConsoleManager {
public:
    typedef std::unordered_map<String, std::shared_ptr<Console>> ConsoleTable;

    static ConsoleManager* get_instance();
    static void initialize_console_manager();
    static void destroy_console_manager();

    void draw_console() const;
    void process() const;
    void switch_console(String console_name);

    void return_console();
    bool running_checker() const;
    void exit_application();

    HANDLE getConsoleHandle() const;

private:
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) = delete;

    static ConsoleManager* sharedInstance;
    
    ConsoleTable console_table;
    bool running = true;
    HANDLE console_handle;

    std::shared_ptr<Console> curr_console;
    std::shared_ptr<Console> prev_console;
};
