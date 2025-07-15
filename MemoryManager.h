#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "MemoryBlock.h"

class MemoryManager{
private:
    int max_overall_mem;
    int mem_per_frame;
    int mem_per_proc;
    std::vector<MemoryBlock> memory_blocks;


public:
    void allocate_memory();
    void deallocate_memory();
};