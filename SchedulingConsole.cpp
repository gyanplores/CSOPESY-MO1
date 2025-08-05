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

#ifdef min
#undef min
#endif
#include <algorithm>

extern MemoryManager memoryManager;  
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

void SchedulingConsole::createAndRunProcess(const std::string& processName, int memSize, const std::string& instructions) {
    std::lock_guard<std::mutex> lock(processMutex);

    // Allocate memory for the process
    if (!memoryManager.allocateMemory(processName)) {
        std::cout << "[MEMORY] Not enough memory for " << processName << ".\n";
        return;
    }

    // Create process object
    Process p(processList.size() + finishedProcesses.size(), 0);
    p.isInMemory = true;

    // Parse instructions (split by ;)
    std::istringstream iss(instructions);
    std::string instr;
    while (std::getline(iss, instr, ';')) {
        if (instr.empty()) continue;

        // Trim leading/trailing spaces
        while (!instr.empty() && isspace(instr.front())) instr.erase(instr.begin());
        while (!instr.empty() && isspace(instr.back())) instr.pop_back();

        ProcessInstructions pi;
        std::istringstream tokenStream(instr);
        std::string type;
        tokenStream >> type;

        if (type == "PRINT") {
            pi.instruction_type = "PRINT";
            std::string msg;
            std::getline(tokenStream, msg);
            if (!msg.empty() && msg[0] == ' ') msg.erase(0, 1);
            pi.instruction_variation = 0;
            pi.constant_string = msg;

        } else if (type == "DECLARE") {
            pi.instruction_type = "DECLARE";
            tokenStream >> pi.var1 >> pi.constant1;

        } else if (type == "ADD") {
            pi.instruction_type = "ADD";
            tokenStream >> pi.var1 >> pi.var2 >> pi.var3;
            pi.instruction_variation = 0;

        } else if (type == "SUBTRACT") {
            pi.instruction_type = "SUBTRACT";
            tokenStream >> pi.var1 >> pi.var2 >> pi.var3;
            pi.instruction_variation = 0;

        } else if (type == "SLEEP") {
            pi.instruction_type = "SLEEP";
            tokenStream >> pi.constant1;

        } else if (type == "WRITE") {
            pi.instruction_type = "WRITE";
            std::string varName;
            std::string addrStr;
            tokenStream >> addrStr >> varName;
            pi.constant1 = std::stoi(addrStr, nullptr, 16); // hex address
            pi.var1 = varName;

        } else if (type == "READ") {
            pi.instruction_type = "READ";
            std::string varName;
            std::string addrStr;
            tokenStream >> varName >> addrStr;
            pi.var1 = varName;
            pi.constant1 = std::stoi(addrStr, nullptr, 16); // hex address

        } else {
            std::cout << "[WARN] Unknown instruction type: " << type << "\n";
            continue;
        }

        p.instructions.push_back(pi);
    }

    // Update process instruction count
    p.instruction_lines_max = p.instructions.size();
    p.remainingTime = p.instructions.size();

    // Add process to scheduling list
    processList.push_back(p);

    // Start scheduler if not running
    if (!isSchedulerRunning) {
        std::cout << "[Scheduler] Starting FCFS Scheduler in background for new process...\n";
        stopRequested = false;
        schedulerThread = std::thread(&SchedulingConsole::runSchedulerInBackground, this);
        schedulerThread.detach();
    }
}

