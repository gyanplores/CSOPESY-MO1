#include "Core.h"
#include <iostream>
#include <fstream>
#include <string.h>

CORE::CORE(int i) : id(i) {}

void CORE::run_print(Process& p) {
    p.setRunning();
    p.current_core = id;

    std::string filename = "Output_files/process_" + std::to_string(p.id) + ".txt";
    std::ofstream file(filename);

    file << "Process Name:  process_" << p.id << "\nLogs:\n\n";

    // Get the global tick (currentQuantumCycle declared in SchedulingConsole.cpp)
    extern int currentQuantumCycle;
    uint32_t& currentTick = (uint32_t&)currentQuantumCycle;

    // Replace with actual instruction loop
    for (int i = p.instruction_lines_current; i < p.instruction_lines_max; i++) {
        // Skip if still sleeping
        if (p.isSleeping(currentTick)) {
            file << p.setCurrentTime() << "  |  CORE: " << id
                 << " | Sleeping until tick " << p.wakeAtTick << "\n";

            p.logInstruction(id);
            std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
            currentTick++;
            continue;
        }

        ProcessInstructions instr;
        instr.instruction_type = "SLEEP";  // Replace this with actual loaded instruction logic
        instr.constant1 = 2;               // sleep for 2 ticks

        std::string log = instr.runInstruction({});  // supply actual memory if needed

        // Check if this is a SLEEP instruction and set wakeAtTick
        if (instr.instruction_type == "SLEEP") {
            p.wakeAtTick = currentTick + instr.constant1;
            p.state = Process::WAITING;
        }

        // Write log to file
        file << p.setCurrentTime() << "  |  CORE: " << id
             << " | " << log << "\n";

        // Save instruction log
        p.logInstruction(id);

        // Advance instruction pointer
        p.instruction_lines_current++;

        // Simulate tick delay
        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
        currentTick++;
    }

    p.setFinished();
}

