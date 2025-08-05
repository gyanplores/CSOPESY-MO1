#pragma once

#include <vector>
#include <mutex>
#include "Process.h"

class ConcurrentVectorBool {
public:
    std::vector<bool> vec;
    std::mutex mtx;

public:
    void push_back(bool b) {
        std::lock_guard<std::mutex> lock(mtx);
        vec.push_back(b);
    }

    bool at(size_t index) {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.at(index);
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.size();
    }

    void setFree(int i) {
        std::lock_guard<std::mutex> lock(mtx);
        vec[i] = 0;
    }
    
    void setUsed(int i) {
        std::lock_guard<std::mutex> lock(mtx);
        vec[i] = 1;
    }
};