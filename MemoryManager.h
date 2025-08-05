#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <unordered_map>
#include <direct.h>   // Windows-specific for _mkdir
#include <cstdint>
#include "Variable.h" // Needed for Var type used by runInstruction

struct MemoryBlock {
    int start;
    int end;
    bool allocated;
    std::string processName;
};

inline std::string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M:%S", ltm);
    return std::string(buffer);
}

inline void createMemoryStampDirectory() {
    _mkdir("memory_stamps");
}

class MemoryManager {
private:
    int totalMemory;
    int maxProcessMemory;
    std::vector<MemoryBlock> memoryBlocks;
    std::vector<Var> memory;  
    std::unordered_map<uint16_t, int> directMemoryMap; 

public:
    MemoryManager(int totalMem, int maxProcMem)
        : totalMemory(totalMem), maxProcessMemory(maxProcMem) {
        memoryBlocks.push_back({0, totalMemory - 1, false, ""});
    }

    bool allocateMemory(const std::string& processName) {
        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
            if (!it->allocated && (it->end - it->start + 1) >= maxProcessMemory) {
                int originalStart = it->start;
                int originalEnd = it->end;

                it->end = originalStart + maxProcessMemory - 1;
                it->allocated = true;
                it->processName = processName;

                if (it->end < originalEnd) {
                    MemoryBlock newBlock = { it->end + 1, originalEnd, false, "" };
                    memoryBlocks.insert(std::next(it), newBlock);
                }

                return true;
            }
        }
        return false;
    }

    void deallocateMemory(const std::string& processName) {
        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
            if (it->allocated && it->processName == processName) {
                it->allocated = false;
                it->processName.clear();

                if (it != memoryBlocks.begin()) {
                    auto prev = std::prev(it);
                    if (!prev->allocated) {
                        prev->end = it->end;
                        it = memoryBlocks.erase(it);
                        it = prev;
                    }
                }

                if (std::next(it) != memoryBlocks.end()) {
                    auto next = std::next(it);
                    if (!next->allocated) {
                        it->end = next->end;
                        memoryBlocks.erase(next);
                    }
                }

                break;
            }
        }
    }

    int calculateExternalFragmentation() {
        int totalFree = 0;
        int largestBlock = 0;

        for (const auto& block : memoryBlocks) {
            if (!block.allocated) {
                int size = block.end - block.start + 1;
                totalFree += size;
                if (size > largestBlock) largestBlock = size;
            }
        }

        return (largestBlock >= maxProcessMemory) ? 0 : totalFree;
    }

    void generateMemorySnapshot(int quantumCycle) {
        createMemoryStampDirectory();
        std::string filename = "memory_stamps/memory_stamp_" + std::to_string(quantumCycle) + ".txt";
        std::ofstream file(filename);

        if (file.is_open()) {
            file << "Timestamp: " << getCurrentTime() << "\n";
            int processCount = 0;
            for (const auto& block : memoryBlocks)
                if (block.allocated)
                    processCount++;

            file << "Number of processes in memory: " << processCount << "\n";
            file << "Total external fragmentation in KB: " << calculateExternalFragmentation() / 1024 << "\n\n";
            file << "---end--- = " << totalMemory << "\n\n";

            for (auto it = memoryBlocks.rbegin(); it != memoryBlocks.rend(); ++it) {
                if (it->allocated) {
                    file << it->end + 1 << "\n";
                    file << it->processName << "\n";
                    file << it->start << "\n\n";
                }
            }

            file << "---start--- = 0\n";
            file.close();
        }
    }

    std::vector<Var>& getMemory() {
        return memory;
    }

    void writeToAddress(uint16_t address, int value) {
        if (address >= totalMemory) {
            std::cerr << "[MemoryManager] Write out of bounds at address " << address << "\n";
            return;
        }
        directMemoryMap[address] = value;
    }

    int readFromAddress(uint16_t address) {
        if (address >= totalMemory) {
            std::cerr << "[MemoryManager] Read out of bounds at address " << address << "\n";
            return 0;
        }
        auto it = directMemoryMap.find(address);
        if (it != directMemoryMap.end()) return it->second;
        return 0;
    }
};
