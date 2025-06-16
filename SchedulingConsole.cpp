#include "SchedulingConsole.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "Core.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <vector>

SchedulingConsole::SchedulingConsole() : Console("SCHEDULING_CONSOLE") {}

void SchedulingConsole::onEnabled() {
    std::cout << "[Scheduler] FCFS Scheduler Console Enabled.\n";
}

void SchedulingConsole::display() {
    std::cout << "Type 'start' to begin FCFS Scheduling or 'exit' to return to main.\n> ";
}

std::string Process::setCurrentTime() {
    time(&timestamp);
    struct tm* timeinfo = localtime(&timestamp);

    std::ostringstream oss;
    oss << "("
        << std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "/"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << "/"
        << (1900 + timeinfo->tm_year) << " ";

    int hour = timeinfo->tm_hour;
    bool is_pm = hour >= 12;
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;

    oss << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_min << ":"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_sec
        << (is_pm ? "PM" : "AM") << ")";

    return oss.str();
}

void SchedulingConsole::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "start") {
        std::cout << "Starting FCFS Scheduler...\n";

        std::vector<Process> processes = Process::print_processes();
        std::vector<CORE> cores;
        std::vector<std::thread> threads;

        for (int i = 0; i < CORE::N_CORE; i++) {
            cores.emplace_back(i);
        }

        int i = 0;
        while (i < processes.size()) {
            for (int j = 0; j < CORE::N_CORE && i < processes.size(); j++, i++) {
                threads.emplace_back(&CORE::run_print, &cores[j], processes[i]);
            }

            for (auto& t : threads) {
                t.join();
            }

            threads.clear();
        }

        std::cout << "All processes finished.\n";

    } else if (cmd == "print") {
        std::cout << "[Scheduler] Running print simulation...\n";

        std::vector<Process> print_process = Process::print_processes();
        std::vector<std::thread> threads;
        std::vector<CORE> cores;

        for (int i = 0; i < CORE::N_CORE; i++) {
            cores.emplace_back(i);
        }

        int i = 0;
        while (i < print_process.size()) {
            for (int j = 0; j < CORE::N_CORE && i < print_process.size(); j++, i++) {
                threads.push_back(std::thread(&CORE::run_print, cores[j], print_process[i]));
            }

            for (auto& t : threads) {
                t.join();
            }

            threads.clear();
        }

        std::cout << "[Scheduler] All print logs written to Output_files/ directory.\n";

    } else if (cmd == "exit") {
        ConsoleManager::get_instance()->return_console();

    } else {
        std::cout << "Unknown command.\n";
    }
}
