#pragma once

#include "Process.h"

#include <vector>
#include <ctime>
#include <string>
#include <thread>

extern uint64_t tick;

class RQ {
    public: 
        inline static int batch_process_freq = 0;

        static ConcurrentVector ProcessQ;
        static ConcurrentVector FinishedQ;

        inline static bool batch_running;

        inline static bool id_counter = 0;
        

    static void initializeRQ();
    static void start_batch();
    static void end_batch();

};

