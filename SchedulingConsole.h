#pragma once
#include "Console.h"
#include "Screen.h"
#include <thread>
#include "Process.h"
#include <mutex>
#include <atomic>

#include "Core.h"
#include "MemoryManager.h"


class SchedulingConsole : public Console {
private:
    std::thread schedulerThread;
    bool isSchedulerRunning = false;
    std::vector<Process> processList;
    std::mutex processMutex;
    std::vector<int> coreUtilization;
    std::mutex utilizationMutex;

    std::vector<Process> finishedProcesses;

    std::atomic<bool> stopRequested{false}; // Proper initialization with brace

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

    void createAndRunProcess(const std::string& processName, int memSize, const std::string& instructions);
};
