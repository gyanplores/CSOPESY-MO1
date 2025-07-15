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
        return vec.empty();
    }

    
};