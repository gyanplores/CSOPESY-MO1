#include "Core.h"
#include "RQ.h"
#include <ctime>
#include <string>
#include <thread>
#include <iostream>
#include <fstream>

ConcurrentVector CORE::runningProcess;  
ConcurrentVectorBool CORE::freeCore;

CORE::CORE(int i) : id(i) {}

void coreRunning(){
    int ticktracker = tick;
    while(true){
        if(ticktracker != tick){
            for (int i = 0; i < CORE::N_CORE; i++){

                //Move instruction line up
                if(CORE::freeCore.at(i) == 1){
                    CORE::runningProcess.incrementcurline(i);
                    CORE::runningProcess.at(i).setCurrentTime();
                    //std::cout<<CORE::runningProcess.at(i).current_cycle<<std::endl;
                }

                //hit max instruction line
                if (CORE::runningProcess.at(i).instruction_lines_current == CORE::runningProcess.at(i).instruction_lines_max){
                    CORE::runningProcess.at(i).setFinished();
                    CORE::freeCore.setFree(i);
                    RQ::FinishedQ.push_back(CORE::runningProcess.at(i));
                }
                //if time cycle reached
                else if (CORE::runningProcess.at(i).current_cycle >= CORE::CYCLE){
                    CORE::runningProcess.at(i).setReady();
                    CORE::freeCore.setFree(i);
                    RQ::ProcessQ.push_back(CORE::runningProcess.at(i));
                }

                

            }
            ticktracker = tick;
        }
    }
}

void CORE::initializeCores(){
    for (int i = 0; i < CORE::N_CORE; i++){
        CORE::freeCore.push_back(0);
        CORE::runningProcess.push_back(Process(-1));
    }
    std::thread t(coreRunning);
    t.detach();
}

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

        std::string log = instr.runInstruction({}, "process_" + std::to_string(p.id));

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

