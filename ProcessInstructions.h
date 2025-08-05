#pragma once
#include "Variable.h"
#include "MemoryManager.h"

#include <string>
#include <vector>
#include <cstdint>

using String = std::string;

extern MemoryManager memoryManager;

class ProcessInstructions {
public:
    // Variables and constants
    String var1 = "";
    String var2 = "";
    String var3 = "";
    String constant_string = "";
    uint16_t constant1 = 0;
    uint16_t constant2 = 0;

    // Instruction metadata
    String instruction_type = "";
    int instruction_variation = 0;

    // Sleep trigger flag
    bool trigger_sleep = false;

    // FOR loop support
    bool isLoop = false;  // true if this instruction is a FOR loop
    int repeatCount = 0;  // how many times to repeat
    std::vector<ProcessInstructions> loopBody;  // instructions inside the loop

    // Public methods
    String runInstruction(MemoryManager& memoryManager, std::vector<Var>& memory, const std::string& processName);
    static std::vector<ProcessInstructions> processForLoop(ProcessInstructions forInstr);  // expand the loop
    int findVar(String varname, std::vector<Var>& memory);

private:
    // Internal instruction logic
    String runPrint(std::vector<Var>& memory);
    String runDeclare(std::vector<Var>& memory);
    void runDeclareBlank(std::vector<Var>& memory);
    String runAdd(std::vector<Var>& memory);
    String runSubtract(std::vector<Var>& memory);
    String runSleep(std::vector<Var>& memory);
    String runWrite(MemoryManager& memoryManager, std::vector<Var>& memory);
    String runRead(MemoryManager& memoryManager, std::vector<Var>& memory);
};
