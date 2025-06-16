#pragma once

#include <vector>
#include <ctime>
#include <string>


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
        int instruction_lines_current = 0; // currentline of the process, updates automatically

        int current_core;

        time_t timestamp; //Is set when its declared and updates when is being processed by core

        ProcessState state = READY;

    public:
        Process(int i, int n);
        std::string setCurrentTime();
        static std::vector<Process> print_processes(); //static method that return the 10 process we need for the homework
        
        //setter function for the state variable
        void setRunning();
        void setFinished();
};