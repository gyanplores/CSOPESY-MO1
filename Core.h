#pragma once

#include <mutex>
#include <thread>
#include <semaphore>
#include <condition_variable>


class CORE {
public:
    inline static const int N_CORE = 2;              // Declared here so it can be used below

private:
    inline static const int DELAY = 100;                // Sleep duration
    int id;
    int n_turns;

    // Variables for run_sync()
    inline static int turn = 1;                         // Tracks the current thread
    inline static std::mutex turn_mutex;                // Ensures a turn is done by one thread at a time 
    inline static std::condition_variable turn_cv;      // Allows thread to wait until its turn
    int counter = 1;                                    // Counts the number of turns of a thread

    // Variables for run_shared()
    inline static const int MAX_ACCESS = 1;             // Max num of threads that can access the shared counter
    inline static std::counting_semaphore<N_THREADS> semaphore{MAX_ACCESS};  // Controls shared counter access
    inline static std::atomic<int> shared_counter = 1;  // Counter shared by all threads

public:
    CORE(int i, int n = 1);

    void run_sync();
    void run_shared();
};
