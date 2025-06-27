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

void SchedulingConsole::runSchedulerInBackground() {
    isSchedulerRunning = true;
    processList.clear();

    std::vector<CORE> cores;
    for (int i = 0; i < CORE::N_CORE; i++) {
        cores.emplace_back(i);
    }

    coreUtilization.resize(CORE::N_CORE, 0);

    {
        std::lock_guard<std::mutex> lock(processMutex);
        processList = Process::print_processes(); // Shared reference for live updates
    }

    std::vector<std::thread> threads;
    int i = 0;
    while (i < processList.size()) {
        for (int j = 0; j < CORE::N_CORE && i < processList.size(); j++, i++) {
            threads.emplace_back(&CORE::run_print, &cores[j], std::ref(processList[i]));

            {
                std::lock_guard<std::mutex> lock(utilizationMutex); 
                coreUtilization[j]++; 
            }
        }

        for (auto& t : threads) t.join();
        threads.clear();
    }

    isSchedulerRunning = false;
    std::cout << "\n[Scheduler] All processes finished in background.\n";
}

void SchedulingConsole::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "start") {
        if (isSchedulerRunning) {
            std::cout << "[Scheduler] FCFS is already running in the background.\n";
        } else {
            std::cout << "[Scheduler] Starting FCFS Scheduler in background...\n";
            schedulerThread = std::thread(&SchedulingConsole::runSchedulerInBackground, this);
            schedulerThread.detach(); // Don't block the main console
        }
    } else if (cmd == "exit") {
        ConsoleManager::get_instance()->switch_console(MAIN);
        
    } 
    else if (cmd == "print") {
        std::cout << "[Scheduler] Starting print simulation in foreground...\n";

        std::vector<Process> print_process = Process::print_processes();
        std::vector<std::thread> threads;
        std::vector<CORE> cores;

        for (int i = 0; i < CORE::N_CORE; i++) {
            cores.emplace_back(i);
        }

        int i = 0;
        while (i < print_process.size()) {
            for (int j = 0; j < CORE::N_CORE && i < print_process.size(); j++, i++) {
                threads.emplace_back(&CORE::run_print, &cores[j], std::ref(print_process[i]));
            }

            for (auto& t : threads) {
                t.join();
            }

            threads.clear();
        }

        std::cout << "[Scheduler] All print logs written to Output_files/ directory.\n";
    }
    else if (cmd == "screen -ls") {
        ConsoleManager::get_instance()->switch_console("SCREEN_VIEW");
    }
    else {
        std::cout << "Unknown command.\n";
    }
}