#pragma once
#include "Console.h"
#include "Screen.h"
#include <unordered_map>
#include <Windows.h>
#include <string>
#include <memory>

using String = std::string;

const String MAIN = "MAIN_CONSOLE";
const String MARQUEE = "MARQUEE_CONSOLE";
const String SCHEDULE = "SCHEDULE_CONSOLE";

class ConsoleManager{
public:
    typedef std::unordered_map<String, std::shared_ptr<Console>> ConsoleTable;
    typedef std::unordered_map<String, std::shared_ptr<Screen>> ScreenTable;

    static ConsoleManager* get_instance();
    ConsoleTable& getConsoleTable();
    static void initialize_console_manager();
    static void destroy_console_manager();

    void initialize_console();

    void draw_console() const;
    void process() const;
    void switch_console(String console_name);

    //void register_screen(std::shared_ptr<Screen> screen_ref);
    //void switch_screen(String screen_name);
    //void unregister_screen(String screen_name);

    void return_console();
    bool running_checker() const;
    void exit_application();

    HANDLE getConsoleHandle() const;

    // Screen management
    void register_screen(std::shared_ptr<Screen> screen);
    void unregister_screen(const String& screen_name);
    void switch_screen(const String& screen_name);
    void return_screen();

    std::shared_ptr<Screen> getCurrentScreen() const;
    ScreenTable& getScreenTable();


    private: 
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {};
    //ConsoleManager& operator=(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance; 
    
    ConsoleTable console_table;
    bool running = true;

    HANDLE console_handle;

    std::shared_ptr<Console> curr_console;
    std::shared_ptr<Console> prev_console;

    ScreenTable screen_table;
    
    std::shared_ptr<Screen> curr_screen = nullptr;
    std::shared_ptr<Screen> prev_screen = nullptr;
};