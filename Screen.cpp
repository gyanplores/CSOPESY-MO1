#include "Screen.h"
#include "ConsoleManager.h"
#include "SchedulingConsole.h"

#include <iomanip>
#include <iostream>
#include <fstream>

Screen::Screen(std::string processName) : Console(processName) {}

void Screen::onEnabled() {
    std::cout << "[Screen] Viewing process output screen.\n";
}

void Screen::display() {
    std::cout << "-----------------------------\n";
}

void Screen::process() {
    std::string cmd;
    std::getline(std::cin, cmd);
    if (cmd == "exit") {
       ConsoleManager::get_instance()->switch_console(SCHEDULE);
    }
    else if (cmd == "report -util"){
        generateFile();
    }
    if (cmd.rfind("process -s", 0) == 0 && cmd.length() > 10) {
        std::string process_name = cmd.substr(10);  
        printProcess(process_name);
    }
}

void Screen::printProcess(std::string processName){
    system("cls");
    std::cout.flush();

    int processId = std::stoi(processName.substr(8));

    auto* scheduler = dynamic_cast<SchedulingConsole*>(
    ConsoleManager::get_instance()->getConsoleTable().at("SCHEDULE_CONSOLE").get());

    if (!scheduler) {
        std::cout << "[Screen] Unable to access scheduler.\n";
        return;
    }

    std::lock_guard<std::mutex> lock(scheduler->processMutex);

    const Process* targetProc = nullptr;
    for (const auto& proc : scheduler->processList) {
        if (proc.id == processId) {
            targetProc = &proc;
            break;
        }
    }

    if (!targetProc) {
        std::cout << "[Screen] Process not found.\n";
        return;
    }

    std::cout << "Process name: " << processName << "\n";
    std::cout << "ID: " << targetProc->id << "\n";

    std::cout << "Logs: \n";
    
    for (const auto& log : targetProc->instructionLogs) {
        std::cout << "(" << log.timestamp << ") Core:" << log.coreId
                  << " \"" << log.message << "\"\n";
    }

    if (targetProc->state == Process::RUNNING) {
            std::cout << "\nCurrent Instruction Line: " << targetProc->instruction_lines_current << "\n";
            std::cout << "Lines of code: " << targetProc->instruction_lines_max << "\n";
        }
    else {
        std::cout << "Finished! \n";
    }

}

    

void Screen::printProcess() const {
    system("cls");
    std::cout.flush();

    auto* scheduler = dynamic_cast<SchedulingConsole*>(
    ConsoleManager::get_instance()->getConsoleTable().at("SCHEDULE_CONSOLE").get());


    if (!scheduler) {
        std::cout << "[Screen] Unable to fetch scheduler info.\n";
        return;
    }

    std::lock_guard<std::mutex> lock(scheduler->processMutex);
    std::lock_guard<std::mutex> locK(scheduler->utilizationMutex);

    int totalUsed = 0;
    for (int val : scheduler->coreUtilization){
        if (val > 0) totalUsed++;
    }

    std::cout << "CPU utilization: " << "\n";
    std::cout << "Cores used: " << totalUsed <<"\n";
    std::cout << "Cores available: " << CORE::N_CORE - totalUsed << "\n";

    std::cout << "-----------------------------\n";
    std::cout << "running processes:\n";

    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::RUNNING) {
            std::cout << "process" << std::setw(2) << std::setfill('0') << proc.id
                      << " (" << ctime(&proc.timestamp)
                      << ") Core: " << proc.current_core << " "
                      << proc.instruction_lines_current << " / "
                      << proc.instruction_lines_max << "\n";
        }
    }

    std::cout << "\nFinished processes:\n";
    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::FINISHED) {
            std::cout << "process" << std::setw(2) << std::setfill('0') << proc.id
                      << " (" << ctime(&proc.timestamp)
                      << ") Finished "
                      << proc.instruction_lines_current << " / "
                      << proc.instruction_lines_max << "\n";
        }
    }
    std::cout << "--------------------------------------------\n";
}

void Screen::generateFile() const {
    std::ofstream file("output_files/csopsey-log.txt");

    if (!file.is_open()) {
        std::cerr << "[Screen] Could not open log file.\n";
        return;
    }

    auto* scheduler = dynamic_cast<SchedulingConsole*>(
        ConsoleManager::get_instance()->getConsoleTable().at("SCHEDULE_CONSOLE").get());

    if (!scheduler) {
        file << "[Screen] Unable to fetch scheduler info.\n";
        return;
    }

    std::lock_guard<std::mutex> lock(scheduler->processMutex);
    std::lock_guard<std::mutex> locK(scheduler->utilizationMutex);

    int totalUsed = 0;
    for (int val : scheduler->coreUtilization){
        if (val > 0) totalUsed++;
    }

    file << "CPU utilization: \n";
    file << "Cores used: " << totalUsed << "\n";
    file << "Cores available: " << CORE::N_CORE - totalUsed << "\n";

    file << "-----------------------------\n";
    file << "Running processes:\n";

    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::RUNNING) {
            file << "process" << std::setw(2) << std::setfill('0') << proc.id
                 << " (" << ctime(&proc.timestamp)
                 << ") Core: " << proc.current_core << " "
                 << proc.instruction_lines_current << " / "
                 << proc.instruction_lines_max << "\n";
        }
    }

    file << "\nFinished processes:\n";
    for (const auto& proc : scheduler->processList) {
        if (proc.state == Process::FINISHED) {
            file << "process" << std::setw(2) << std::setfill('0') << proc.id
                 << " (" << ctime(&proc.timestamp)
                 << ") Finished "
                 << proc.instruction_lines_current << " / "
                 << proc.instruction_lines_max << "\n";
        }
    }

    file << "--------------------------------------------\n";
    file.close();
    std::cout << "[Screen] Report saved to csopsey-log.txt\n";
}

