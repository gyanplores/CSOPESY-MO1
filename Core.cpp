#include "Core.h"
#include <iostream>
#include <fstream>
#include <string.h>

CORE::CORE(int i) : id(i) {}

void CORE::run_print(Process p){
    p.setRunning();
    int i;

    std::string filename = "process_";
    filename.append(std::to_string(p.id));
    filename.append(".txt");

    std::ofstream file(filename);

    file<<"Process Name:  process_"<<p.id<<"\n";
    file<<"Logs:\n\n";

    for (i = p.instruction_lines_current; i <= p.instruction_lines_max; i++) {
        file << p.setCurrentTime() << "  |  CORE: " << this->id << " Hello World from process_"<< p.id << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
        p.instruction_lines_current = i;
    } 

    p.instruction_lines_current = i;
    p.setFinished();
}