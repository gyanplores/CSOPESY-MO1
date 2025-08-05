#include "ProcessInstructions.h"
#include "MemoryManager.h"
#include <string>
#include <vector>
#include <sstream>

using String = std::string;

extern MemoryManager memoryManager;

String ProcessInstructions::runInstruction(MemoryManager& memoryManager, std::vector<Var>& memory, const std::string& processName) {
    if (this->instruction_type == "PRINT") {
        return runPrint(memory);
    } else if (this->instruction_type == "DECLARE") {
        memoryManager.loadPageIfNeeded(processName, 0);
        return runDeclare(memory);
    } else if (this->instruction_type == "ADD") {
        memoryManager.loadPageIfNeeded(processName, 0);
        return runAdd(memory);
    } else if (this->instruction_type == "SUBTRACT") {
        memoryManager.loadPageIfNeeded(processName, 0);
        return runSubtract(memory);
    } else if (this->instruction_type == "SLEEP") {
        return runSleep(memory);
    } else if (this->instruction_type == "WRITE") {
        return runWrite(memoryManager, memory);
    } else if (this->instruction_type == "READ") {
        return runRead(memoryManager, memory);
    }

    return "Unknown instruction.";
}

int ProcessInstructions::findVar(String varname, std::vector<Var>& memory) {
    for (int i = 0; i < (int)memory.size(); i++) {
        if (memory[i].name == varname) {
            return i;
        }
    }
    return -1;
}

String ProcessInstructions::runPrint(std::vector<Var>& memory) {
    std::string output = constant_string;

    // Replace any variable names in the string with their values
    for (auto& var : memory) {
        size_t pos = 0;
        while ((pos = output.find(var.name, pos)) != std::string::npos) {
            output.replace(pos, var.name.length(), std::to_string(var.value));
            pos += std::to_string(var.value).length();
        }
    }

    switch (this->instruction_variation) {
        case 0: return output;
        case 1: return output + std::to_string(this->constant1);
        case 2: return std::to_string(this->constant1) + output;
    }
    return "Invalid PRINT variation.";
}

String ProcessInstructions::runDeclare(std::vector<Var>& memory) {
    memory.push_back(Var(this->var1, this->constant1));
    String log = "Variable " + this->var1 + " declared with value " + std::to_string(this->constant1);
    return log;
}

void ProcessInstructions::runDeclareBlank(std::vector<Var>& memory) {
    memory.push_back(Var(this->var1, 0));
}

String ProcessInstructions::runAdd(std::vector<Var>& memory) {
    String log = "Added ";
    switch (this->instruction_variation) {
        case 0: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);
            if (i >= 0 && j >= 0 && k >= 0) {
                memory[i].value = memory[j].value + memory[k].value;
            }
            log += this->var2 + " and " + this->var3 + ". Results stored in " + this->var1;
            return log;
        }
        case 1: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            if (i >= 0 && j >= 0) {
                memory[i].value = memory[j].value + this->constant1;
            }
            log += this->var2 + " and " + std::to_string(this->constant1) + ". Results stored in " + this->var1;
            return log;
        }
        case 2: {
            int i = findVar(this->var1, memory);
            if (i >= 0) {
                memory[i].value = this->constant1 + this->constant2;
            }
            log += std::to_string(this->constant1) + " and " + std::to_string(this->constant2) + ". Results stored in " + this->var1;
            return log;
        }
    }
    return "Invalid ADD variation.";
}

String ProcessInstructions::runSubtract(std::vector<Var>& memory) {
    String log = "Subtracted ";
    switch (this->instruction_variation) {
        case 0: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);
            if (i >= 0 && j >= 0 && k >= 0) {
                memory[i].value = memory[j].value - memory[k].value;
            }
            log += this->var2 + " from " + this->var3 + ". Results stored in " + this->var1;
            return log;
        }
        case 1: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            if (i >= 0 && j >= 0) {
                memory[i].value = memory[j].value - this->constant1;
            }
            log += this->var2 + " from " + std::to_string(this->constant1) + ". Results stored in " + this->var1;
            return log;
        }
        case 2: {
            int i = findVar(this->var1, memory);
            if (i >= 0) {
                memory[i].value = this->constant1 - this->constant2;
            }
            log += std::to_string(this->constant1) + " and " + std::to_string(this->constant2) + ". Results stored in " + this->var1;
            return log;
        }
    }
    return "Invalid SUBTRACT variation.";
}

String ProcessInstructions::runSleep(std::vector<Var>& memory) {
    String log = "Process instructed to sleep for " + std::to_string(this->constant1) + " quantum cycles.";
    this->trigger_sleep = true;
    return log;
}

std::vector<ProcessInstructions> ProcessInstructions::processForLoop(ProcessInstructions forInstr) {
    std::vector<ProcessInstructions> expanded;
    for (int i = 0; i < forInstr.repeatCount; ++i) {
        for (const auto& instr : forInstr.loopBody) {
            expanded.push_back(instr);
        }
    }
    return expanded;
}

String ProcessInstructions::runWrite(MemoryManager& memoryManager, std::vector<Var>& memory) {
    int idx = findVar(this->var1, memory);
    if (idx == -1) return "Variable " + var1 + " not found.";
    int value = memory[idx].value;

    memoryManager.writeToAddress(this->constant1, value);

    std::stringstream ss;
    ss << std::hex << this->constant1;
    return "Wrote value " + std::to_string(value) + " to memory address 0x" + ss.str();
}

String ProcessInstructions::runRead(MemoryManager& memoryManager, std::vector<Var>& memory) {
    int value = memoryManager.readFromAddress(this->constant1);
    memory.push_back(Var(this->var1, value));

    std::stringstream ss;
    ss << std::hex << this->constant1;
    return "Read value " + std::to_string(value) + " from memory address 0x" + ss.str() +
           " into variable " + var1;
}
