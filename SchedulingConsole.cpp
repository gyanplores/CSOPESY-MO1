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
