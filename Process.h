#pragma once
#include <fstream>
#include <mutex>

class Process {
public:
    enum State { READY, RUNNING, WAITING, FINISHED };

    Process(int id, int maxPrints);
    ~Process();

<<<<<<< Updated upstream
    // called by a worker; writes one entry, returns true when done
    bool executePrint(int coreId);
=======
        int burstTime;
        int remainingTime;

        int burstTime;
        int remainingTime;

        time_t timestamp;
>>>>>>> Stashed changes

    // open the per-process log file
    void openLogFile();

    int getId() const;
    void setState(State s);

private:
    int id;
    int maxPrints;
    int printedCount = 0;
    State state;

    std::ofstream logFile;
    std::mutex   fileMutex;
};
