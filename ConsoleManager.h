#pragma once
#include "Console.h"
#include "Screen.h"
#include <unordered_map>
#include <Windows.h>
#include <string>

using String = std::string;

const String MAIN = "MAIN_CONSOLE";
const String MARQUEE = "MARQUEE_CONSOLE";
const String SHEDULE = "SCHEDULE_CONSOLE";

class ConsoleManager{
public:
    typedef std::unordered_map<String, std::shared_ptr<Console>> ConTable;

    static ConsoleManager* get_instance();
    static void initialize_console_manager();
    static void destroy_console_manager();

    void draw_console() const;
    void switch_console(String console_name);
    void return_console();
    void running_checker() const;
    void exit_application();

    void register_screen(std::shard_ptr<Screen> screen_ref);
    void switch_screen(String screen_name);
    void unregister_screen(String screen_name);
private: 
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {};
    ConsoleManager& operator=(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance;

    ConTable conTable;

    bool running = true;
};