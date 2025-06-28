#pragma once
#include "Console.h"
#include "Process.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <memory>

class SchedulingConsole : public Console {
<<<<<<< Updated upstream
=======
private:
    std::thread schedulerThread;
    bool isSchedulerRunning = false;
    std::vector<Process> processList;
    std::mutex processMutex;
    std::vector<int> coreUtilization;
    std::mutex utilizationMutex;

    int quantum = 2;

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
public:
    SchedulingConsole();
    ~SchedulingConsole();

    void onEnabled() override;
    void display() override;
    void process() override;

private:
    void startScheduler();
    void stopScheduler();
    void schedulerLoop();
    void workerLoop(int coreId);
    void printStatus();

    // All 10 processes
    std::vector<std::shared_ptr<Process>> processes;

    // FCFS ready queue
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::mutex readyMutex;
    std::condition_variable readyCv;

    // Threads
    std::thread schedulerThread;
    std::vector<std::thread> workerThreads;
    std::atomic<bool> running{false};

    // State for screen -ls
    std::mutex stateMutex;
    std::vector<std::shared_ptr<Process>> finished;
    std::shared_ptr<Process> currentRunning;
};
