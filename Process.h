#pragma once

#include <vector>
#include <ctime>
#include <string>
#include "ProcessInstructions.h"  // Required to define the instruction type

using String = std::string;

struct InstructionLog {
    std::string timestamp;
    int coreId;
    std::string message;
};

class Process {
public:
    enum ProcessState {
        READY = 0,
        RUNNING,
        WAITING,
        FINISHED
    };

public:
    int id; // ID of process
    int instruction_lines_max; // total instructions
    int instruction_lines_current = 0;

    int current_core;

    int burstTime;
    int remainingTime;

    bool isInMemory = false;
    
    std::string customName;

    time_t timestamp;

    ProcessState state = READY;

    std::vector<InstructionLog> instructionLogs;
    std::vector<String> var_names;

    // Hold the actual instructions to run
    std::vector<ProcessInstructions> instructions;

    // For SLEEP(X)
    int wakeAtTick = -1;

public:
    Process(int i, int n);
    std::string setCurrentTime();
    static std::vector<Process> print_processes();

    void setRunning();
    void setFinished();

    void logInstruction(int coreId);

    void setSleep(int wakeTick) {   
        state = WAITING;
        wakeAtTick = wakeTick;
    }

    bool isSleeping(int currentTick) const {
        return wakeAtTick != -1 && currentTick < wakeAtTick;
    }

    void runNextInstruction(std::vector<Var>& memory);
};
