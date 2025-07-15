#pragma once


#include <vector>
#include <ctime>
#include <string>
#include <random>

using String = std::string;

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
        int instruction_lines_max; //all lines of instructions of process - check threads.size
        int instruction_lines_current = 0;

        int current_core;
        int current_cycle; //used for round robin

        inline static int min_ins;
        inline static int max_ins;

        time_t timestamp; //date created
        time_t timestamp_last; //date last running

        ProcessState state = READY;

        std::vector<InstructionLog> instructionLogs;

        //std::vector<ProcessInstructions> threads;

        std::vector<String> var_names;


    public:
        Process(int i, int n);
        Process(int i);
        std::string setCurrentTime();
        static std::vector<Process> print_processes();
        
        void setRunning();
        void setFinished();
        void setReady();


        void logInstruction(int coreId);
};