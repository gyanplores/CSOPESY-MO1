#include "SchedulingConsole.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "Core.h"

#undef min
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
<<<<<<< Updated upstream
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
=======
    /*
    {
        std::lock_guard<std::mutex> lock(processMutex);
        processList = Process::print_processes();

        for (auto& p : processList) {
            p.burstTime = 5 + p.id;
            p.remainingTime = p.burstTime;
        }
    }*/
>>>>>>> Stashed changes

    std::vector<std::thread> threads;
    int i = 0;
    while (i < processList.size()) {
        for (int j = 0; j < CORE::N_CORE && i < processList.size(); j++, i++) {
            threads.emplace_back(&CORE::run_print, &cores[j], std::ref(processList[i]));

<<<<<<< Updated upstream
            {
                std::lock_guard<std::mutex> lock(utilizationMutex); 
                coreUtilization[j]++; 
            }
        }

        for (auto& t : threads) t.join();
        threads.clear();
=======
        Process& p = processList.front();

        if (!p.isInMemory) {
            if (!memoryManager.allocateMemory("process_" + std::to_string(p.id))) {
                std::cout << "[MEMORY] Not enough memory for process_" << p.id << ". Waiting...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                processList.erase(processList.begin());
                continue;
            }
            p.isInMemory = true;
        }

        // Instead of copying, modify p directly.
        int runTime = std::min(quantum, p.remainingTime);
        p.remainingTime -= runTime;

        std::this_thread::sleep_for(std::chrono::milliseconds(runTime * 100));

        if (stopRequested) {
            std::cout << "[Scheduler] Stopping scheduler as requested.\n";
            memoryManager.deallocateMemory("process_" + std::to_string(p.id));
            p.isInMemory = false;
            break;
        }

        // Generate snapshot after each quantum cycle
        currentQuantumCycle++;
        memoryManager.generateMemorySnapshot(currentQuantumCycle);
        std::cout << "[MEMORY] Snapshot saved for quantum: " << currentQuantumCycle << "\n";

        if (p.remainingTime > 0) {
            processList.push_back(p);  // push back the same process
        } else {
            std::cout << "[DONE] P" << p.id << " completed.\n";
            memoryManager.deallocateMemory("process_" + std::to_string(p.id));
            p.isInMemory = false;
        }

        processList.erase(processList.begin());  // Only erase after processing


>>>>>>> Stashed changes
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
    else {
        std::cout << "Unknown command.\n";
    }
}