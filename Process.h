#pragma once

#include <vector>


class Process {
    public:
        enum ProcessState{
            READY = 0,
            RUNNING,
            WAITING,
            FINISHED
        };
    private:
        int id; //ID of process
        int instruction_lines_max; //all lines of instructions of process - defaults to print currently
        int instruction_lines_current = 0;

        int current_core;

        ProcessState state = READY;

    public:
        Process(int i, int n);
        static std::vector<Process> print_processes() 
};