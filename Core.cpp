#include "Core.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <thread>

extern MemoryManager memoryManager;

CORE::CORE(int i) : id(i) {}

void CORE::run_print(Process& p) {
    p.setRunning();
    p.current_core = id;

    std::string filename = "Output_files/process_" + std::to_string(p.id) + ".txt";
    std::ofstream file(filename);

    file << "Process Name:  process_" << p.id << "\nLogs:\n\n";

    // Get the global tick
    extern int currentQuantumCycle;
    uint32_t& currentTick = (uint32_t&)currentQuantumCycle;

    // Shared process memory (persistent across instructions)
    std::vector<Var> memory;

    // Loop through instructions
    while (p.instruction_lines_current < p.instruction_lines_max) {
        // Skip if process is sleeping
        if (p.isSleeping(currentTick)) {
            file << p.setCurrentTime() << "  |  CORE: " << id
                 << " | Sleeping until tick " << p.wakeAtTick << "\n";
            p.logInstruction(id);
            std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
            currentTick++;
            continue;
        }

        std::string processName = "process_" + std::to_string(p.id);

        // Get the current instruction
        ProcessInstructions& instr = p.instructions[p.instruction_lines_current];

        // Run the instruction
        std::string log = instr.runInstruction(memoryManager, memory, processName);

        // If it's a SLEEP instruction, set wake time
        if (instr.instruction_type == "SLEEP" && instr.trigger_sleep) {
            p.setSleep(currentTick + instr.constant1);
        }

        // Write log to file
        file << p.setCurrentTime() << "  |  CORE: " << id
             << " | " << log << "\n";

        // Save instruction log
        p.logInstruction(id);

        // Move to next instruction
        p.instruction_lines_current++;

        // Simulate CPU delay
        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
        currentTick++;
    }

    p.setFinished();
}

