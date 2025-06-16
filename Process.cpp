#include "Process.h"
#include <iostream>
#include <vector>
#include <string>

Process::Process(int i, int n) : id(i), instruction_lines_max(n) { time(&timestamp); }

//return the a vector of processes for week 6 homework

std::vector<Process> Process::print_processes() {
    static std::vector<Process> v;

    for (int i = 0; i < 10; i++){
        v.push_back(Process(i, 10));
    }

    return v;
}

void Process::setRunning(){
    this->state = RUNNING;
}

void Process::setFinished(){
    this->state = FINISHED;
}

std::string Process::setCurrentTime(){
    time(&(this->timestamp));
    time_t t = this->timestamp;
    std::string thetime = ctime(&t);
    thetime.erase(thetime.find('\n', 0), 1);
    return thetime;
}