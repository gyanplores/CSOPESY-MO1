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

    for (int i = p.instruction_lines_current; i <= p.instruction_lines_max; i++) {
        file << p.setCurrentTime() << "  |  CORE: " << this->id
             << " Hello World from process_" << p.id << "\n";

        p.instruction_lines_current = i;

        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
    }

    p.setFinished();
}
