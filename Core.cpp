#include "Core.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include "MemoryManager.h"

CORE::CORE(int i) : id(i) {}

void CORE::run_print(Process& p) {
    p.setRunning();
    p.current_core = id;

    std::string filename = "Output_files/process_" + std::to_string(p.id) + ".txt";
    std::ofstream file(filename);
    file << "Process Name:  process_" << p.id << "\nLogs:\n\n";

    extern int currentQuantumCycle;
    uint32_t& currentTick = (uint32_t&)currentQuantumCycle;

    // Assume we have memory from MemoryManager
    extern MemoryManager memoryManager;
    auto& memory = memoryManager.getMemory();

    while (p.instruction_lines_current < p.instruction_lines_max) {
        if (p.isSleeping(currentTick)) {
            std::string logMsg = p.setCurrentTime() + "  |  CORE: " + std::to_string(id)
                               + " | Sleeping until tick " + std::to_string(p.wakeAtTick);
            file << logMsg << "\n";
            p.logInstruction(id, logMsg);
            std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
            currentTick++;
            continue;
        }

        ProcessInstructions& instr = p.instructions[p.instruction_lines_current];
        std::string result = instr.runInstruction(memory);

        if (instr.instruction_type == "SLEEP") {
            p.wakeAtTick = currentTick + instr.constant1;
            p.state = Process::WAITING;
        }

        file << p.setCurrentTime() << "  |  CORE: " << id << " | " << result << "\n";
        p.logInstruction(id, result);

        p.instruction_lines_current++;
        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
        currentTick++;
    }

    p.setFinished();
}
