#pragma once

#include <mutex>
#include <thread>
#include <semaphore>
#include <condition_variable>
#include "Process.h"

class CORE {
public:
    inline static const int N_CORE = 4;              // Declared here so it can be used below

private:
    inline static const int DELAY = 1;                // Sleep duration
    int id;

public:
    CORE(int i);
    void run_print(Process p);
};
