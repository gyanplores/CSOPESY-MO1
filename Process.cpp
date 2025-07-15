#include "Process.h"
#include "ConsoleManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());


Process::Process(int i, int n) : id(i), instruction_lines_max(n) { time(&timestamp); }

int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

Process::Process(int i) : id(i){ 
    time(&timestamp); 
    timestamp_last = timestamp;

    int rand_proc = random(min_ins, max_ins);

    /*for (int i = 0; i < rand_proc; i++){
        ProcessInstructions pi = ProcessInstructions();
        pi.instruction_type = "PRINT";

        threads.push_back(pi);
    }*/

    instruction_lines_max = rand_proc;


}

//return the a vector of processes for week 6 homework

std::vector<Process> Process::print_processes() {
    static std::vector<Process> v;

    for (int i = 0; i < 10; i++){
        v.push_back(Process(i, min_ins + (rand() % (max_ins - min_ins + 1))));
    }

    return v;
}

void Process::setRunning(){
    this->state = RUNNING;
}

void Process::setFinished(){
    this->state = FINISHED;
}

void Process::setReady(){
    this->state = READY;
}

std::string Process::setCurrentTime(){
    time(&(this->timestamp_last));
    time_t t = this->timestamp_last;
    std::string thetime = ctime(&t);
    thetime.erase(thetime.find('\n', 0), 1);
    return thetime;
}

void Process::logInstruction(int coreId) {
    time_t now;
    time(&now);
    char timeBuffer[80];
    strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", localtime(&now));

    InstructionLog log = {
        timeBuffer,
        coreId
    };

    instructionLogs.push_back(log);
}
