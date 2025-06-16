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

    for (int i = 0; i < CORE::N_CORE; i++) {
        threads.push_back(std::thread(&CORE::run_print, cores[i], print_process[i]));
    }

    for (int i = 0; i < CORE::N_CORE; i++) {
        threads[i].join();
    }

    /*
    for (int i = 1; i <= CORE::N_CORE; i++) {
        workers.push_back(ThreadWorker(i, n_turns));
    }

    // Multiple threads independently counting 1 to n_turns 
    std::cout << "--- Running asynchronous threads ---" << std::endl;
    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads.push_back(std::thread(&ThreadWorker::run_async, &workers[i]));
    }

    for (int i = 0; i < ThreadWorker::N_THREADS; i++) {
        threads[i].join();
    }
    threads.clear();
    std::cout << "--- Finished asynchronous threads ---" << std::endl;
    std::cout << std::endl;
    */
}