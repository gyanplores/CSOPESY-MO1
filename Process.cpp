#include "Process.h"
#include <iostream>
#include <vector>

Process::Process(int i, int n) : id(i), instruction_lines_max(n) {}

//return the a vector of processes for week 6 homework

static std::vector<Process> print_processes() {
    static std::vector<Process> v;

    for (int i = 0; i < 10; i++){
        v.push_back(Process(i, 100));
    }
    
    return v;
}