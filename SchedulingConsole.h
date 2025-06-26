#pragma once
#include "Console.h"
#include "Screen.h"
#include <thread>
#include "Process.h"
#include <mutex>
#include "Core.h"

class SchedulingConsole : public Console {
private:
    std::thread schedulerThread;
    bool isSchedulerRunning = false;
    std::vector<Process> processList;
    std::mutex processMutex;
    std::vector<int> coreUtilization;
    std::mutex utilizationMutex;

public:
    SchedulingConsole();
    friend class Screen;
    void updateProcess(const Process& p);
    void onEnabled() override;
    void display() override;
    void process() override;
    void runSchedulerInBackground(); 
};
