#include "SchedulingConsole.h"
#include "ConsoleManager.h"
#include "Process.h"
#include "Core.h"
#include "MemoryManager.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <vector>

#ifdef min
#undef min
#endif
#include <algorithm>

MemoryManager memoryManager(16384, 4096);  // total memory, memory per process
int currentQuantumCycle = 0;

SchedulingConsole::SchedulingConsole() : Console("SCHEDULING_CONSOLE") {}

void SchedulingConsole::onEnabled() {
    std::lock_guard<std::mutex> lock(processMutex);
    processList.clear();

    // Use test processes with PRINT → SLEEP → PRINT
    processList = Process::print_processes();  // Includes SLEEP(X)

    std::cout << "[Scheduler] Round Robin Scheduler initialized with SLEEP test processes.\n";

    stopRequested = false;
}

void SchedulingConsole::display() {
    std::cout << "Type 'start' to begin FCFS Scheduling or 'exit' to return to main.\n> ";
}

void SchedulingConsole::runSchedulerInBackground() {
    isSchedulerRunning = true;

    while (isSchedulerRunning && !stopRequested) {
        std::lock_guard<std::mutex> lock(processMutex);
        if (processList.empty()) break;

        Process& p = processList.front();

        if (p.isSleeping(currentQuantumCycle)) {
            std::cout << "[Scheduler] P" << p.id << " is sleeping until tick " << p.wakeAtTick << ".\n";
            processList.push_back(p);
            processList.erase(processList.begin());
            continue;
        }

        if (!p.isInMemory) {
            if (!memoryManager.allocateMemory("process_" + std::to_string(p.id))) {
                std::cout << "[MEMORY] Not enough memory for process_" << p.id << ". Waiting...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                processList.erase(processList.begin());
                continue;
            }
            p.isInMemory = true;
        }

        int timeUsed = 0;
        while (timeUsed < quantum && p.state != Process::FINISHED && !p.isSleeping(currentQuantumCycle)) {
            p.runNextInstruction(memoryManager.getMemory());
            timeUsed++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        p.remainingTime -= timeUsed;

        if (stopRequested) {
            std::cout << "[Scheduler] Stopping scheduler as requested.\n";
            memoryManager.deallocateMemory("process_" + std::to_string(p.id));
            p.isInMemory = false;
            break;
        }

        currentQuantumCycle++;
        memoryManager.generateMemorySnapshot(currentQuantumCycle);
        std::cout << "[MEMORY] Snapshot saved for quantum: " << currentQuantumCycle << "\n";

        if (p.remainingTime > 0 && p.state != Process::FINISHED) {
            processList.push_back(p);
        } else {
            std::cout << "[DONE] P" << p.id << " completed.\n";
            memoryManager.deallocateMemory("process_" + std::to_string(p.id));
            p.isInMemory = false;

            finishedProcesses.push_back(p);
        }

        processList.erase(processList.begin());
    }

    std::cout << "[Scheduler] Round Robin scheduling completed.\n";
    isSchedulerRunning = false;
}

void SchedulingConsole::stopScheduler() {
    stopRequested = true;
}

void SchedulingConsole::startScheduler() {
    stopRequested = false;
    schedulerThread = std::thread(&SchedulingConsole::runSchedulerInBackground, this);
    schedulerThread.detach();
}

void SchedulingConsole::createProcessByName(const std::string& name) {
    std::lock_guard<std::mutex> lock(processMutex);

    int newId = processList.size();
    Process p(newId, 3); // 3 instructions default
    p.customName = name;

    ProcessInstructions instr;
    instr.instruction_type = "PRINT";
    instr.constant_string = "Process " + name + " started.";
    p.instructions.push_back(instr);

    processList.push_back(p);
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
