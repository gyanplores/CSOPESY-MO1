<<<<<<< Updated upstream
#include "SchedulingConsole.h"
#include "ConsoleManager.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

typedef std::chrono::system_clock Clock;

SchedulingConsole::SchedulingConsole()
  : Console("SCHEDULE_CONSOLE")
{}

SchedulingConsole::~SchedulingConsole() {
    stopScheduler();
}

void SchedulingConsole::onEnabled() {
    std::cout
      << "--- FCFS Scheduler Console ---\n"
      << "Commands:\n"
      << "  scheduler-test   Start the 4-core FCFS test\n"
      << "  scheduler-stop   Stop scheduler and return\n"
      << "  screen -ls       List running/finished\n"
      << "  return           Back to main console\n";
}

void SchedulingConsole::display() {
    std::cout << "(scheduler)> ";
}

void SchedulingConsole::process() {
    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "scheduler-test") {
        startScheduler();
    }
    else if (cmd == "scheduler-stop") {
        stopScheduler();
    }
    else if (cmd == "screen -ls") {
        printStatus();
    }
    else if (cmd == "return") {
        stopScheduler();
        ConsoleManager::get_instance()->return_console();
    }
    else {
        std::cout << "Unknown command\n";
    }
}

void SchedulingConsole::startScheduler() {
    if (running) {
        std::cout << "Scheduler already running\n";
        return;
    }
    // Prepare processes
    processes.clear();
    finished.clear();
    while (!readyQueue.empty()) readyQueue.pop();

    for (int i = 1; i <= 10; ++i) {
        auto p = std::make_shared<Process>(i, 100);
        p->openLogFile();
        processes.push_back(p);
        readyQueue.push(p);
    }

    running = true;
    // Launch threads
    schedulerThread = std::thread(&SchedulingConsole::schedulerLoop, this);
    for (int c = 0; c < 4; ++c) {
        workerThreads.emplace_back(&SchedulingConsole::workerLoop, this, c);
    }

    std::cout << "Scheduler started\n";
}

void SchedulingConsole::stopScheduler() {
    if (!running) return;
    running = false;
    readyCv.notify_all();

    if (schedulerThread.joinable())
        schedulerThread.join();
    for (auto& t : workerThreads)
        if (t.joinable())
            t.join();
    workerThreads.clear();

    std::cout << "Scheduler stopped\n";
}

void SchedulingConsole::schedulerLoop() {
    while (running) {
        std::unique_lock<std::mutex> lk(readyMutex);
        readyCv.wait(lk, [&] { return !readyQueue.empty() || !running; });
        if (!running) break;

        auto proc = readyQueue.front();
        readyQueue.pop();

        {
            std::lock_guard<std::mutex> st(stateMutex);
            currentRunning = proc;
            proc->setState(Process::RUNNING);
        }

        // wake up one worker
        lk.unlock();
        readyCv.notify_one();
    }
}

void SchedulingConsole::workerLoop(int coreId) {
    while (running) {
        std::shared_ptr<Process> proc;
        {
            std::unique_lock<std::mutex> lk(readyMutex);
            readyCv.wait(lk, [&] { return (!readyQueue.empty() && running); });
            if (!running) break;
            proc = readyQueue.front();
            readyQueue.pop();
        }

        bool finishedNow = proc->executePrint(coreId);

        {
            std::lock_guard<std::mutex> st(stateMutex);
            if (finishedNow) {
                proc->setState(Process::FINISHED);
                finished.push_back(proc);
            } else {
                proc->setState(Process::READY);
                std::lock_guard<std::mutex> lk(readyMutex);
                readyQueue.push(proc);
                readyCv.notify_one();
            }
            currentRunning.reset();
        }
    }
}

void SchedulingConsole::printStatus() {
    std::lock_guard<std::mutex> st(stateMutex);
    std::cout << "Running processes:\n";
    if (currentRunning) {
        std::cout << "  process" << currentRunning->getId() << "\n";
    }
    std::cout << "Finished processes:\n";
    for (auto& p : finished) {
        std::cout << "  process" << p->getId() << "\n";
    }
}
=======
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

        // Initialize burst & remaining time
        for (auto& p : processList) {
            p.burstTime = 5 + p.id;
            p.remainingTime = p.burstTime;
        }
    }

    while (isSchedulerRunning) {
        std::lock_guard<std::mutex> lock(processMutex);

        if (processList.empty()) break;

        Process p = processList.front();
        processList.erase(processList.begin());

        int runTime = std::min(quantum, p.remainingTime);
        p.remainingTime -= runTime;

        std::cout << "[CPU] Running P" << p.id << " for " << runTime << " units\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(runTime * 100));

        if (p.remainingTime > 0) {
            processList.push_back(p);
        } else {
            std::cout << "[DONE] P" << p.id << " completed.\n";
        }
    }

    std::cout << "[Scheduler] Round Robin scheduling completed.\n";
    isSchedulerRunning = false;
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
>>>>>>> Stashed changes
