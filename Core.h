#pragma once

#include <mutex>
#include <thread>
#include <semaphore>
#include <condition_variable>
#include "Process.h"

class CORE {
public:
    inline static int N_CORE = 4;              // Declared here so it can be used below
    inline static int DELAY = 100;                // Sleep duration

private:
    int id;

public:
    CORE(int i);
    void set_CoreN(int n);
    void run_print(Process& p);
};
