#include "RQ.h"
#include "CORE.h"

#include <vector>
#include <ctime>
#include <string>
#include <thread>
#include <iostream>

using namespace std;

std::vector<Process> RQ::ProcessQ;
std::vector<Process> RQ::FinishedQ;

void rqRunning(){
    int ticktracker = tick;
    int i = 0;
    while(true){
        if (ticktracker != tick){

            //this is for scheduler start
            if (RQ::batch_running){
                if (tick % RQ::batch_process_freq){
                    RQ::ProcessQ.push_back(Process(i));
                    i++;
                    //RQ::id_counter = 10;
                    //ID counting up is bugged on static for now
                }
            }

            for (int i = 0; i < CORE::N_CORE; i++){
                if(!RQ::ProcessQ.empty()){
                    //push into cores if free
                    if(CORE::freeCore[i] == 0){
                        RQ::ProcessQ[0].setRunning();
                        RQ::ProcessQ[0].current_core = i;
                        CORE::runningProcess[i] = RQ::ProcessQ[0];
                        CORE::freeCore[i] = 1; //set core as being used
                        RQ::ProcessQ.erase(RQ::ProcessQ.begin());
                    }
                }
            }
            ticktracker = tick;
        }
    }
}

/*
void push_back_batch(){
    int ticktracker = tick;
    int i = 0;
    while(RQ::batch_running){
        if (ticktracker != tick){
            if (tick % RQ::batch_process_freq){
                RQ::ProcessQ.push_back(Process(i));
                i++;
                //RQ::id_counter = 10;
                //ID counting up is bugged on static for now
            }
            ticktracker = tick;
        }
            
    }
}*/

void RQ::initializeRQ(){
    thread t2(rqRunning);
    t2.detach();
}

void RQ::start_batch(){
    batch_running = true;
}

void RQ::end_batch(){
    batch_running = false;
}