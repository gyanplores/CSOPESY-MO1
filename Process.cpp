#include "Process.h"
#include "MemoryManager.h"
#include <iostream>
#include <vector>
#include <string>

extern MemoryManager memoryManager;
extern int currentQuantumCycle; // To access the scheduler's global quantum tick

Process::Process(int i, int n) : id(i), instruction_lines_max(n) {
    time(&timestamp);

    burstTime = instruction_lines_max;
    remainingTime = burstTime;
}

std::vector<Process> Process::print_processes() {
    std::vector<Process> v;

    for (int i = 0; i < 10; i++) {
        Process p(i, 0);

        // Build loop body
        std::vector<ProcessInstructions> loopBody;

        // PRINT "Hello from process_X"
        ProcessInstructions instr1;
        instr1.instruction_type = "PRINT";
        instr1.instruction_variation = 0;
        instr1.constant_string = "Hello from process_" + std::to_string(i);
        loopBody.push_back(instr1);

        // SLEEP(2)
        ProcessInstructions instr2;
        instr2.instruction_type = "SLEEP";
        instr2.constant1 = 2;
        loopBody.push_back(instr2);

        // PRINT "Process X woke up"
        ProcessInstructions instr3;
        instr3.instruction_type = "PRINT";
        instr3.instruction_variation = 0;
        instr3.constant_string = "Process " + std::to_string(i) + " woke up.";
        loopBody.push_back(instr3);

        // Wrap it in FOR([...], repeats=2)
        ProcessInstructions forInstr;
        forInstr.instruction_type = "FOR";
        forInstr.repeatCount = 2;
        forInstr.loopBody = loopBody;

        // Expand the loop and assign to process
        std::vector<ProcessInstructions> expanded = forInstr.processForLoop(forInstr);

        p.instructions = expanded;
        p.instruction_lines_max = p.instructions.size();

        v.push_back(p);
    }

    return v;
}

void Process::setRunning() {
    this->state = RUNNING;
}

void Process::setFinished() {
    this->state = FINISHED;
}

std::string Process::setCurrentTime() {
    time(&(this->timestamp));
    time_t t = this->timestamp;
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

void Process::runNextInstruction(std::vector<Var>& memory) {
    if (instruction_lines_current >= instruction_lines_max) {
        setFinished();
        return;
    }

    ProcessInstructions& instr = instructions[instruction_lines_current];
    std::string result = instr.runInstruction(memoryManager, memory);

    if (instr.instruction_type == "SLEEP" && instr.trigger_sleep) {
        this->setSleep(currentQuantumCycle + instr.constant1);  // delay process
    }

    std::cout << "[P" << id << "] " << result << "\n";

    instruction_lines_current++;
}
