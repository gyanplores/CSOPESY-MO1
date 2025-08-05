#include "RQ.h"
#include "CORE.h"

#include <vector>
#include <ctime>
#include <string>
#include <thread>
#include <iostream>

using namespace std;

ConcurrentVector RQ::ProcessQ;
ConcurrentVector RQ::FinishedQ;

void rqRunning(){
    int ticktracker = tick;
    int i = 0;
    while(true){
        if (ticktracker != tick){

            //this is for scheduler start
            

            for (int i = 0; i < CORE::N_CORE; i++){
                if(!RQ::ProcessQ.empty()){
                    //push into cores if free
                    if(CORE::freeCore.at(i) == 0){
                        CORE::freeCore.setUsed(i); //set core as being used
                        RQ::ProcessQ.at(i).setRunning();
                        RQ::ProcessQ.setCore(0,i);

                        CORE::runningProcess.changeAt(i, RQ::ProcessQ.at(0));

                        RQ::ProcessQ.pop_first();
                    }
                }
            }
            ticktracker = tick;
        }
    }
}


void push_back_batch(){
    int ticktracker = tick;
    int i = 0;
    while(RQ::batch_running){
        if (ticktracker != tick){
            if (RQ::batch_running){
                if (tick % RQ::batch_process_freq){
                    RQ::ProcessQ.push_back(Process(i));
                    i++;
                    //RQ::id_counter = 10;
                    //ID counting up is bugged on static for now
                }
            }
        }
            
    }
}

void RQ::initializeRQ(){
    thread t2(rqRunning);
    t2.detach();
}

void RQ::start_batch(){
    batch_running = true;
    thread t(push_back_batch);
    t.detach();
}

void RQ::end_batch(){
    batch_running = false;
}