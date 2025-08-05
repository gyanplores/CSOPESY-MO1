#pragma once
#include "Console.h"
#include "Screen.h"
#include <thread>
#include "Process.h"
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>

#include "Core.h"
#include "MemoryManager.h"

class SchedulingConsole : public Console {
private:
    std::thread schedulerThread;
    bool isSchedulerRunning = false;

    std::vector<std::shared_ptr<Process>> processList;  
    std::vector<std::shared_ptr<Process>> finishedProcesses; 

    std::mutex processMutex;
    std::vector<int> coreUtilization;
    std::mutex utilizationMutex;

    std::atomic<bool> stopRequested{false};

    MemoryManager memoryManager{16384, 4096};
    int quantum = 4;
    int currentQuantumCycle = 0;

public:
    SchedulingConsole();

    // Prevent copying to avoid atomic copy errors
    SchedulingConsole(const SchedulingConsole&) = delete;
    SchedulingConsole& operator=(const SchedulingConsole&) = delete;

    friend class Screen;
    void updateProcess(const Process& p);
    void onEnabled() override;
    void display() override;
    void process() override;
    void runSchedulerInBackground(); 
    void stopScheduler(); // Method to stop scheduler

    std::vector<std::shared_ptr<Process>>& getProcessList() { return processList; }
    std::vector<std::shared_ptr<Process>>& getFinishedProcesses() { return finishedProcesses; } 
};
