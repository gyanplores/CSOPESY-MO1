#include "Core.h"
#include <iostream>
#include <string.h>

CORE::CORE(int i) : id(i) {}

void CORE::run_print(Process p){
    p.setRunning();
    int i;
    for (i = p.instruction_lines_current; i <= p.instruction_lines_max; i++) {
        std::cout <<"\n" << p.setCurrentTime() << "  |  CORE: " << this->id << " Hello World from screen_01! thanks to process_"<< p.id << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(CORE::DELAY));
    } 

    p.instruction_lines_current = i;
}