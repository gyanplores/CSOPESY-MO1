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
    std::lock_guard<std::mutex> lock(processMutex);
    processList.clear();

    for (int i = 0; i < 5; ++i) {
        Process p(i, 0);
        p.burstTime = 5 + i;
        p.remainingTime = p.burstTime;
        processList.push_back(p);
    }

    std::cout << "[Scheduler] Round Robin Scheduler initialized with dummy processes.\n";

    stopRequested = false;
    schedulerThread = std::thread(&SchedulingConsole::runSchedulerInBackground, this);
    schedulerThread.detach();
}

void SchedulingConsole::display() {
    std::cout << "Type 'start' to begin FCFS Scheduling or 'exit' to return to main.\n> ";
}

void SchedulingConsole::runSchedulerInBackground() {
    isSchedulerRunning = true;

    {
        std::lock_guard<std::mutex> lock(processMutex);
        processList = Process::print_processes();

        for (auto& p : processList) {
            p.burstTime = 5 + p.id;
            p.remainingTime = p.burstTime;
        }
    }

    while (isSchedulerRunning && !stopRequested) {
        std::lock_guard<std::mutex> lock(processMutex);
        if (processList.empty()) break;

        Process p = processList.front();
        processList.erase(processList.begin());

        int runTime = std::min(quantum, p.remainingTime);
        p.remainingTime -= runTime;

       
        std::this_thread::sleep_for(std::chrono::milliseconds(runTime * 100));

        if (stopRequested) {
            std::cout << "[Scheduler] Stopping scheduler as requested.\n";
            break;
        }

        if (p.remainingTime > 0) {
            processList.push_back(p);
        } else {
            std::cout << "[DONE] P" << p.id << " completed.\n";
        }
    }

    std::cout << "[Scheduler] Round Robin scheduling completed.\n";
    isSchedulerRunning = false;
}

void SchedulingConsole::stopScheduler() {
    stopRequested = true;
}

void SchedulingConsole::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "start") {
        if (isSchedulerRunning) {
            std::cout << "[Scheduler] FCFS is already running in the background.\n";
        } else {
            std::cout << "[Scheduler] Starting FCFS Scheduler in background...\n";
            stopRequested = false;
            schedulerThread = std::thread(&SchedulingConsole::runSchedulerInBackground, this);
            schedulerThread.detach();
        }
    } else if (cmd == "scheduler-stop") {
        if (isSchedulerRunning) {
            stopScheduler();
        } else {
            std::cout << "[Scheduler] Scheduler is not running.\n";
        }
    } else if (cmd == "exit") {
        ConsoleManager::get_instance()->switch_console(MAIN);
    } else if (cmd == "print") {
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
    } else {
        std::cout << "Unknown command.\n";
    }
}
