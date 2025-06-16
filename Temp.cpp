#include "Core.h"
#include "Process.h"
#include <iostream>
#include <vector>

// Compilation:
// g++ -std=c++20 threads_demo.cpp ThreadWorker.cpp -o threads_demo


int main() {
    std::vector<Process> print_process = Process::print_processes();
    std::vector<std::thread> threads;
    std::vector<CORE> cores;

    int n_turns = 10;
    
    for (int i = 0; i < 4; i++){
        cores.push_back(CORE(i));
    }

    cores[0].run_print(print_process[0]);

    //beginning of printing process
    int i = 0;
    while (i < 10){
        for (int j = 0; j < CORE::N_CORE && i < 10; j++) {
            threads.push_back(std::thread(&CORE::run_print, cores[j], print_process[i]));
            std::cout<<i<<std::endl;
            i++;
            
        }

        for (int j = 0; j < CORE::N_CORE && j < threads.size(); j++) {
            threads[j].join();
        }

        threads.clear();
    }
    
}