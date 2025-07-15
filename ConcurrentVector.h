#pragma once

#include <vector>
#include <mutex>
#include "Process.h"

class ConcurrentVector {
public:
    std::vector<Process> vec;
    std::mutex mtx;

public:
    void push_back(Process p) {
        std::lock_guard<std::mutex> lock(mtx);
        vec.push_back(p);
    }

    Process at(size_t index) {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.at(index);
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.size();
    }

    void pop_first() {
        std::lock_guard<std::mutex> lock(mtx);
        vec.erase(vec.begin());
    }

    void incrementcurline(int i){
        std::lock_guard<std::mutex> lock(mtx);
        vec[i].instruction_lines_current++;
        vec[i].current_cycle++;
    }

    bool empty(){
        std::lock_guard<std::mutex> lock(mtx);
        return vec.empty();
    }

    void setCore(int i, int j){
        std::lock_guard<std::mutex> lock(mtx);
        vec[i].current_core = j;
    }

    void changeAt(int i, Process p){
        std::lock_guard<std::mutex> lock(mtx);
        vec[i] = p;
    }

    time_t getTimeStampLast(int i){
        std::lock_guard<std::mutex> lock(mtx);
        return vec[i].timestamp_last;
    }

    void resetCycle(int i){
        std::lock_guard<std::mutex> lock(mtx);
        vec[i].current_cycle = 0;
    }

    std::vector<Process> duplicate(){
        std::lock_guard<std::mutex> lock(mtx);
        return vec;
    }
    
};