#pragma once
#include "Variable.h"

#include <string>
#include <vector>
#include <cstdint>

using String = std::string;


class ProcessInstructions {
    public:
        String var1 = "";
        String var2 = "";
        String var3 = "";
        String constant_string = "";
        uint16_t constant1 = 0;
        uint16_t constant2 = 0;

        String instruction_type = "";
        int instruction_variation = 0;

    public:

    String runInstruction(std::vector<Var> memory);
    std::vector<ProcessInstructions> processForLoop(std::vector<ProcessInstructions> block );

    int findVar(String varname, std::vector<Var> memory);
   
    private:

    String runPrint(std::vector<Var> memory);
    String runDeclare(std::vector<Var> memory);
    void runDeclareBlank(std::vector<Var> memory);
    String runAdd(std::vector<Var> memory);
    String runSubtract(std::vector<Var> memory);
    String runSleep(std::vector<Var> memory);

};