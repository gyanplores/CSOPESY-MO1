#include <iostream>     
#include <stdlib.h>     
#include <string>
#include <sstream>
#include <vector>       
#include <ctime>
#include <iomanip>

// Enum class representing all possible commands
enum class StringCode {
    exit,
    clear,
    help,
    initialize,
    screen,
    scheduler_test,
    scheduler_stop,
    report_util,
    unknown
};

// Forward declaration of function that will process user commands
void processCommand(std::string command);

// Forward declaration of function that tokenizes the command
void tokenizeCommand(std::string command);

// Forward declaration of function that will print the console screen
void printScreenConsole(const std::string& name);

// Forward declaration of function that will loop the console for screen
void consoleLoop(const std::string& name);

// Forward declaration of function that will terminate program
void shutdownAndCleanup();

// Foward declaration of helper function for header print
void printHeader();

// Function that starts the command loop and handles user input
void enterMainLoop() {
    bool running = true;

    printHeader();

    std::cout << "Welcome to CSOPESY commandline!" << "\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear screen, and 'help' to view commands" << "\n";

    while (running) {
        std::string x = "";
        std::cout << "Type a command: ";
        std::getline(std::cin, x); // Takes commands with spaces into consideration
        processCommand(x);  // Process the entered command
    }
}

// Function that maps input string to the corresponding enum command
StringCode hashString(const std::string& str) {
    if (str == "exit") return StringCode::exit;
    if (str == "clear") return StringCode::clear;
    if (str == "help") return StringCode::help;
    if (str == "initialize") return StringCode::initialize;
    if (str == "screen -r") return StringCode::screen;
    if (str == "scheduler-test") return StringCode::scheduler_test;
    if (str == "scheduler-stop") return StringCode::scheduler_stop;
    if (str == "report-util") return StringCode::report_util;
    return StringCode::unknown;  // Return unknown if command doesn't match any known ones
}

// Function to tokenize the string command
void tokenizeCommand(const std::string& command, std::string& setcommand, std::string& name){
    std::istringstream fullstring(command);
    std::vector<std::string> tokens;
    std::string token;

    while(getline(fullstring, token, ' ')){
        tokens.push_back(token);
    }

    if(tokens.size() >= 2){
        setcommand = tokens[0] + " " + tokens[1];   //  Specifically made for "screen -r"
        name = tokens[2];
    }else{
        setcommand = tokens[0];
    }
}

// Function that performs the action associated with each command
void processCommand(std::string command) {
    std::string setcommand, name;
    
    tokenizeCommand(command, setcommand, name); //  Tokenize command
    
    std::cout << "Command is: " << setcommand << " Name is:" << name << "\n\n"; //  For Testing

    switch (hashString(setcommand)) {
        case StringCode::exit:
            shutdownAndCleanup();  // Exit program
            break;
        case StringCode::clear:
            system("cls");  // Clear screen (Windows-specific)
            printHeader(); // Reprint Header
            break;
        case StringCode::help:
            // Display available commands
            std::cout << "Available commands:\n"
                      << "  exit            - Quit the program\n"
                      << "  clear           - Clear the screen\n"
                      << "  help            - Show this help message\n"
                      << "  initialize      - Run initialization routine\n"
                      << "  screen          - Perform screen-related action\n"
                      << "  scheduler-test  - Test the scheduler\n"
                      << "  scheduler-stop  - Stop the scheduler\n"
                      << "  report-util     - Run report utility\n";
            break;
        case StringCode::initialize:
            std::cout << "initialize command recognized. Doing something.\n";
            break;
        case StringCode::screen:
            system("cls");
            consoleLoop(name);
            system("cls");
            printHeader();
            break;
        case StringCode::scheduler_test:
            std::cout << "scheduler-test command recognized. Doing something.\n";
            break;
        case StringCode::scheduler_stop:
            std::cout << "scheduler-stop command recognized. Doing something.\n";
            break;
        case StringCode::report_util:
            std::cout << "report-util command recognized. Doing something.\n";
            break;
        case StringCode::unknown:
            std::cout << "error: unknown command. Please type a valid command.\n";
            break;
    }
}

// Helper function to print the header
void printHeader(){
    //  ASCII text header guide
    /* _______   _______    _______   _______   _______   _______    __    __ 
      /  ___  \ /   __  \  /   _   \ /   _   \ /   ____| /   __  \  |  |  |  |
      | |   \_| |  |__|__| |  | |  | |  |_|  | |  |____  |  |__|__| |  \__|  |
      | |    _  \_____   \ |  | |  | |   ____/ |   ____| \_____   \  \____   |
      | |___/ | |  |__|  | |  |_|  | |  |      |  |____  |  |__|  | |  |__|  |
      \_______/ \_______/  \_______/ |__|      \_______| \_______/  \_______/ 
    */

    std::cout << " _______   _______    _______   _______   _______   _______    __    __\n" 
              << "/  ___  \\ /   __  \\  /   _   \\ /   _   \\ /   ____| /   __  \\  |  |  |  |\n"
              << "| |   \\_| |  |__|__| |  | |  | |  |_|  | |  |____  |  |__|__| |  \\__|  |\n"
              << "| |    _  \\_____   \\ |  | |  | |   ____/ |   ____| \\_____   \\  \\____   |\n"
              << "| |___/ | |  |__|  | |  |_|  | |  |      |  |____  |  |__|  | |  |__|  |\n"
              << "\\_______/ \\_______/  \\_______/ |__|      \\_______| \\_______/  \\_______/\n\n";
}

void printScreenConsole(const std::string& name){
    
    // Gets the current date time
    time_t currTime = time(0);
    char buff[100];
    tm* localTime = localtime(&currTime);
    
    // Places the datetime variables into a format
    strftime(buff, sizeof(buff), "(%m/%d/%Y, %I:%M:%S %P)", localTime);

    std::cout << "Process Name: " << name << "\nID: 0" << "\nLogs:" << std::endl;
    std::cout << buff << "  Core: 0" << std::endl;
    std::cout << buff << "  Core: 1" << std::endl;
    std::cout << buff << "  Core: 2" << std::endl;
    std::cout << buff << "  Core: 3" << std::endl;
    std::cout << buff << "  Core: 4" << std::endl;

    std::cout << "\nCurrent instruction line: 1234\n" << "Lines of code: 1738" << std::endl; 
}

void consoleLoop(const std::string& name){
    bool running = true;
    printScreenConsole(name); // Prints the layout for the console

    while(running){  // While loop to allow the users to run commands
        std::string x = "";
        std::cout << "Type a command: ";
        std::getline(std::cin, x);

        switch(hashString(x)){
            case StringCode::exit:  // Exit stops the while loop
                running = false;
                break;
            default:
                std::cout << "error: unknown command. Please type a valid command.\n";
        }
        
    }
}


// Function to perform shutdown tasks and exit the program
void shutdownAndCleanup() {
    exit(0);  // Exit the program immediately
}

// Main function, entry point of the program
int main() {
    enterMainLoop();  // Start the command loop
    return 0;
}
