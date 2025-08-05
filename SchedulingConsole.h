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

    static inline std::string SCHEDULER_TYPE = "rr";
    static inline int QUANTUM_CYCLES = 4;
    static inline int BATCH_PROCESS_FREQ = 1;
    static inline int MIN_INS = 100;
    static inline int MAX_INS = 100;
    static inline int MAX_MEMORY = 16384;
    static inline int MEM_PER_FRAME = 1024;
    static inline int MIN_MEM_PER_PROC = 4096;
    static inline int MAX_MEM_PER_PROC = 4096;

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
