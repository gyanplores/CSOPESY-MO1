#pragma once

#include <vector>
#include <ctime>
#include <string>

struct InstructionLog {
    std::string timestamp;
    int coreId;
    std::string message;
};

class Process {
    public:
        enum ProcessState{
            READY = 0,
            RUNNING,
            WAITING,
            FINISHED
        };
    public:
        int id; //ID of process
        int instruction_lines_max; //all lines of instructions of process - defaults to print currently
        int instruction_lines_current = 0;

        int current_core;

        time_t timestamp;

        ProcessState state = READY;

        std::vector<InstructionLog> instructionLogs;

    public:
        Process(int i, int n);
        std::string setCurrentTime();
        static std::vector<Process> print_processes();
        
        void setRunning();
        void setFinished();

        void logInstruction(int coreId);
};