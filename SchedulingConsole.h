#pragma once
#include "Console.h"
#include "Screen.h"
#include <thread>
#include "Process.h"
#include <mutex>
#include "Core.h"
#include <atomic> // For stop flag

class SchedulingConsole : public Console {
private:
    std::thread schedulerThread;
    bool isSchedulerRunning = false;
    std::vector<Process> processList;
    std::mutex processMutex;
    std::vector<int> coreUtilization;
    std::mutex utilizationMutex;

    std::atomic<bool> stopRequested{false}; // Proper initialization with brace

    int quantum = 2;

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
};
