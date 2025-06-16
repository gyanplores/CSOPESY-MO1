#include "Process.h"
#include <chrono>
#include <ctime>
#include <iomanip>

Process::Process(int i, int n)
  : id(i), maxPrints(n), printedCount(0), state(READY)
{}

Process::~Process() {
    if (logFile.is_open()) logFile.close();
}

void Process::openLogFile() {
    std::lock_guard<std::mutex> lk(fileMutex);
    logFile.open("process_" + std::to_string(id) + ".txt", std::ios::out);
}

bool Process::executePrint(int coreId) {
    std::lock_guard<std::mutex> lk(fileMutex);

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&t);

    logFile << "("
            << std::put_time(&tm, "%m/%d/%Y %I:%M:%S%p")
            << ") Core:" << coreId
            << " Print#" << (printedCount + 1)
            << "\n";

    ++printedCount;
    return (printedCount >= maxPrints);
}

int Process::getId() const {
    return id;
}

void Process::setState(State s) {
    state = s;
}
