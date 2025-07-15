#include <Windows.h>
#include <string>
#include <memory>
#include <thread>
#include <iostream>

#include <fstream>
#include <sstream>
#include <ctime>

#include "CORE.h"
#include "RQ.h"

uint64_t clock_cycle;
uint64_t tick;
uint64_t quantum_cycles;
uint64_t min_ins;
uint64_t max_ins;
uint64_t delate_per_exec;

void tick_counter() {
    while(true){
        tick++;
        std::this_thread::sleep_for(std::chrono::milliseconds(clock_cycle));
    }
}

void entranceMessage(){
    std::cout << " _______   _______    _______   _______   _______   _______    __    __\n" 
              << "/  ___  \\ /   __  \\  /   _   \\ /   _   \\ /   ____| /   __  \\  |  |  |  |\n"
              << "| |   \\_| |  |__|__| |  | |  | |  |_|  | |  |____  |  |__|__| |  \\__|  |\n"
              << "| |    _  \\_____   \\ |  | |  | |   ____/ |   ____| \\_____   \\  \\____   |\n"
              << "| |___/ | |  |__|  | |  |_|  | |  |      |  |____  |  |__|  | |  |__|  |\n"
              << "\\_______/ \\_______/  \\_______/ |__|      \\_______| \\_______/  \\_______/\n\n";
    std::cout << "Welcome to CSOPESY commandline!" << "\n";
    std::cout << "Type 'exit' to quit, 'clear' to clear screen, and 'help' to view commands" << "\n";
}

void display(){
    std::cout << "Type a command: ";
}

void print_ls(){
    std::cout << "CPU Utilization: ???%"<<std::endl;
    std::cout << "Cores Used:"<<std::endl;
    std::cout << "Cores available:"<<std::endl;

    std::cout<< "\n---------------------------------------------"<<std::endl;
    std::cout<< "Running Processes"<<std::endl;
    for (int i = 0; i < CORE::runningProcess.size(); i++){
        //CORE::runningProcess.at(i).state == 1
        if (true){
            time_t stamp = CORE::runningProcess.getTimeStampLast(i);
            char* timeStr = std::ctime(&stamp);
            timeStr[strcspn(timeStr, "\n")] = '\0';

            std::cout<<"CORE "<<CORE::runningProcess.at(i).current_core<<": "<<"\t";
            std::cout<<"process"<<CORE::runningProcess.at(i).id<<"\t"<<timeStr<<"\t";
            std::cout<<"READY"<<"\t"<<CORE::runningProcess.at(i).instruction_lines_current<<" / ";
            std::cout<<CORE::runningProcess.at(i).instruction_lines_max<<std::endl;
        }
    }
    std::cout<<std::endl;

    std::cout<< "Finished Processes"<<std::endl;
    for (int i = 0; i < RQ::ProcessQ.size(); i++){
        if (RQ::ProcessQ.at(i).state == 3){
            time_t stamp = CORE::runningProcess.getTimeStampLast(i);
            char* timeStr = std::ctime(&stamp);
            timeStr[strcspn(timeStr, "\n")] = '\0';


            std::cout<<"process"<<RQ::ProcessQ.at(i).id<<"\t"<<timeStr<<"\t";
            std::cout<<"READY"<<"\t"<<RQ::ProcessQ.at(i).instruction_lines_current<<" / ";
            std::cout<<RQ::ProcessQ.at(i).instruction_lines_max<<std::endl;
        }
    }
    std::cout<<std::endl;

    std::cout<< "Ready Processes"<<std::endl;
    for (int i = 0; i < RQ::ProcessQ.size(); i++){
        if (RQ::ProcessQ.at(i).state == 0){
            time_t stamp = CORE::runningProcess.getTimeStampLast(i);
            char* timeStr = std::ctime(&stamp);
            timeStr[strcspn(timeStr, "\n")] = '\0';


            std::cout<<"process"<<RQ::ProcessQ.at(i).id<<"\t"<<timeStr<<"\t";
            std::cout<<"READY"<<"\t"<<RQ::ProcessQ.at(i).instruction_lines_current<<" / ";
            std::cout<<RQ::ProcessQ.at(i).instruction_lines_max<<std::endl;
        }
    }
    std::cout<<std::endl;
    std::cout<< "\n---------------------------------------------"<<std::endl;
    std::cout<<std::endl;
}


int main(){

    std::cout<<"Awaiting Initialization"<<std::endl;
    
    while(true){
        std::string temp;
        std::cin>>temp;
        if (temp == "initialize"){
            break;
        }
    }

    std::ifstream file("config.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        std::string value;
        int value_int;

        iss >> key;
        iss >> value;

        if (key == "clock-speed") {
            value_int = stoi(value);
            CORE::DELAY = value_int;
            clock_cycle = value_int;
            std::cout << "[Config] Set clock speed to " << CORE::DELAY << "\n";
        }
        if (key == "num-cpu") {
            value_int = stoi(value);
            CORE::N_CORE = value_int;
            std::cout << "[Config] Set number of cores to " << CORE::N_CORE << "\n";
        }
        if (key == "scheduler") {
            std::cout << "[Config] Set scheduler to " << value << "\n";
        }
        if (key == "quantum-cycles") {
            value_int = stoi(value);
            CORE::CYCLE = value_int;
            std::cout << "[Config] Set quantum cycles to " << CORE::CYCLE << "\n";
        }
        if (key == "batch-process-freq") {
            value_int = stoi(value);
            RQ::batch_process_freq = value_int;
            std::cout << "[Config] Set batch-process-freq to " << RQ::batch_process_freq << "\n";
        }
        if (key == "min-ins") {
            value_int = stoi(value);
            Process::min_ins = value_int;
            std::cout << "[Config] Set min_ins to " << Process::min_ins << "\n";
        }
        if (key == "max-ins") {
            value_int = stoi(value);
            Process::max_ins = value_int;
            std::cout << "[Config] Set max_ins to " << Process::max_ins << "\n";
        }
        if (key == "delay-per-exec"){
            value_int = stoi(value);
            CORE::DELAY = value_int;
            std::cout << "[Config] set delay to " << CORE::DELAY << "\n";
        }
        if (key == "delay-per-exec"){
            value_int = stoi(value);
            CORE::DELAY = value_int;
            std::cout << "[Config] set delay to " << CORE::DELAY << "\n";
        }
    }

    std::thread t(tick_counter);
    t.detach();

    file.close();

    CORE::initializeCores();
    RQ::initializeRQ();
    entranceMessage();
    

    while(true){
        display();
        std::string input;
        std::cin>>input;
        if (input == "YES"){
            std::cout<<tick<<std::endl;
        }
        else if (input == "scheduler-start"){
            RQ::start_batch();
            std::cout<<"Starting Test..."<<std::endl;
        }
        else if (input == "scheduler-stop"){
            RQ::end_batch();
            std::cout<<"Stopping test batch generation."<<std::endl;
        }
        else if (input == "screen-ls"){
            std::thread print(print_ls);
            print.join();
        }

        input = "";
    }
    return 0;
}