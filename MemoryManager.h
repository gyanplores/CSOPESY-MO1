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

struct PageFrame {
    bool allocated = false;
    std::string processName = "";
    int pageNumber = -1;  // Which virtual page is loaded here
};

struct PageTableEntry {
    bool present = false;       // In RAM?
    int frameIndex = -1;        // Which frame (if present)
    int backingStoreIndex = -1; // Where in file if swapped out
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

inline void createBackingStoreFile() {
    std::ofstream out("Output_files/csopesy-backing-store.txt", std::ios::app);
    out.close();
}

class MemoryManager {
private:
    int totalMemory;
    int maxProcessMemory;
    std::vector<MemoryBlock> memoryBlocks;
    std::vector<Var> memory;  // Used by instruction execution
    std::unordered_map<uint16_t, int> directMemoryMap; 

    // Demand paging
    int pageSize = 1024; // 1 KB
    int numFrames;
    std::vector<PageFrame> frameTable;
    std::vector<std::vector<PageTableEntry>> processPageTables;

public:
    MemoryManager(int totalMem, int maxProcMem)
        : totalMemory(totalMem), maxProcessMemory(maxProcMem) {
        memoryBlocks.push_back({0, totalMemory - 1, false, ""});
        numFrames = totalMemory / pageSize;
        frameTable.resize(numFrames);
        createMemoryStampDirectory();
        createBackingStoreFile();
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

                // Allocate page table for this process
                int numPages = maxProcessMemory / pageSize;
                processPageTables.push_back(std::vector<PageTableEntry>(numPages));

                return true;
            }
        }
        return false;
    }

    void deallocateMemory(const std::string& processName) {
        // Deallocate from legacy memory blocks
        for (auto it = memoryBlocks.begin(); it != memoryBlocks.end(); ++it) {
            if (it->allocated && it->processName == processName) {
                it->allocated = false;
                it->processName = "";

                // Merge with previous block if free
                if (it != memoryBlocks.begin()) {
                    auto prev = std::prev(it);
                    if (!prev->allocated) {
                        prev->end = it->end;
                        it = memoryBlocks.erase(it);
                        it = prev;
                    }
                }

                // Merge with next block if free
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

        // Clear any frames in frameTable owned by this process
        for (auto& frame : frameTable) {
            if (frame.allocated && frame.processName == processName) {
                frame = {}; // clear the frame
            }
        }

        // Invalidate entries in this process's page table
        int pid = getProcessIndex(processName);
        if (pid != -1 && pid < processPageTables.size()) {
            for (auto& entry : processPageTables[pid]) {
                entry.present = false;
                entry.frameIndex = -1;
            }
        }

    // Optional: Clear the page table vector entirely (free memory)
    // processPageTables[pid].clear();
}


    int loadPageIfNeeded(const std::string& processName, int pageIndex) {
        int pid = getProcessIndex(processName);
        if (pid == -1 || pageIndex >= processPageTables[pid].size()) return -1;

        auto& entry = processPageTables[pid][pageIndex];

        if (entry.present) return entry.frameIndex;

        // PAGE FAULT
        int frame = findFreeFrame();
        if (frame == -1) frame = evictPage();

        frameTable[frame] = { true, processName, pageIndex };
        entry.present = true;
        entry.frameIndex = frame;

        logPageLoad(processName, pageIndex, frame);
        return frame;
    }

    int findFreeFrame() const {
        for (int i = 0; i < frameTable.size(); ++i) {
            if (!frameTable[i].allocated) return i;
        }
        return -1;
    }

    int evictPage() {
        for (int i = 0; i < frameTable.size(); ++i) {
            if (frameTable[i].allocated) {
                std::string victimProc = frameTable[i].processName;
                int victimPage = frameTable[i].pageNumber;
                int victimPID = getProcessIndex(victimProc);
                if (victimPID != -1) {
                    auto& victimEntry = processPageTables[victimPID][victimPage];

                    logPageEviction(victimProc, victimPage, i);

                    victimEntry.present = false;
                    victimEntry.frameIndex = -1;
                }
                frameTable[i] = {};
                return i;
            }
        }
        return 0;
    }

    void logPageEviction(const std::string& processName, int page, int frame) {
        std::ofstream file("Output_files/csopesy-backing-store.txt", std::ios::app);
        if (file.is_open()) {
            file << getCurrentTime() << " - Evicted " << processName
                 << " page " << page << " from frame " << frame << " to backing store\n";
            file.close();
        }
    }

    int getProcessIndex(const std::string& name) {
        for (int i = 0; i < memoryBlocks.size(); ++i) {
            if (memoryBlocks[i].allocated && memoryBlocks[i].processName == name) {
                return i;
            }
        }
        return -1;
    }

    void logPageLoad(const std::string& processName, int page, int frame) {
        std::ofstream file("Output_files/csopesy-backing-store.txt", std::ios::app);
        if (file.is_open()) {
            file << getCurrentTime() << " - Loaded " << processName
                 << " page " << page << " into frame " << frame << "\n";
            file.close();
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

    int getTotalMemory() const {
        return totalMemory;
    }

    int getUsedMemory() const {
        int used = 0;
        for (const auto& block : memoryBlocks) {
            if (block.allocated) {
                used += (block.end - block.start + 1);
            }
        }
        return used;
    }

    struct BlockInfo {
        std::string processName;
        int blockSize;
        int startAddress;
    };

    std::vector<BlockInfo> getAllocatedBlocks() const {
        std::vector<BlockInfo> result;
        for (const auto& block : memoryBlocks) {
            if (block.allocated) {
                result.push_back(BlockInfo{
                    block.processName,
                    block.end - block.start + 1,
                    block.start
                });
            }
        }
        return result;
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
