#pragma once

#include <mutex>
#include <thread>
// #include <semaphore>
#include <condition_variable>
#include <thread>
#include "Process.h"
#include "ConcurrentVectorBool.h"
#include "ConcurrentVector.h"

extern uint64_t tick;

class CORE {
public:
    inline static int N_CORE = 4;              // Declared here so it can be used below
    inline static int DELAY = 100;                // Sleep duration currently being used in main


    inline static int CYCLE = 0;//READY QUEYE delay 

    static ConcurrentVector runningProcess;  
    static ConcurrentVectorBool freeCore;


private:
    int id;

public:
    CORE(int i);
    static void initializeCores();
    void set_CoreN(int n);
    void run_print(Process& p);
};
