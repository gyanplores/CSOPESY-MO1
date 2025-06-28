#include "ProcessInstructions.h"
#include <string>
#include <vector>

using String = std::string;

String ProcessInstructions::runInstruction(std::vector<Var> memory) {
    if (this->instruction_type == "PRINT") {
        return runPrint(memory);
    } else if (this->instruction_type == "DECLARE") {
        return runDeclare(memory);
    } else if (this->instruction_type == "ADD") {
        return runAdd(memory);
    } else if (this->instruction_type == "SUBTRACT") {
        return runSubtract(memory);
    }
    return "Unknown instruction.";
}

int ProcessInstructions::findVar(String varname, std::vector<Var> memory) {
    for (int i = 0; i < memory.size(); i++) {
        if (memory[i].name == varname) {
            return i;
        }
    }
    return -1; // fixed infinite recursion
}

String ProcessInstructions::runPrint(std::vector<Var> memory) {
    switch (this->instruction_variation) {
        case 0: {
            return this->constant_string;
        }
        case 1: {
            return this->constant_string + std::to_string(this->constant1);
        }
        case 2: {
            return std::to_string(this->constant1) + this->constant_string;
        }
    }
    return "Invalid PRINT variation.";
}

String ProcessInstructions::runDeclare(std::vector<Var> memory) {
    memory.push_back(Var(this->var1, this->constant1));
    String log = "Variable ";
    log += this->var1;
    log += " declared with value ";
    log += std::to_string(this->constant1);
    return log;
}

void ProcessInstructions::runDeclareBlank(std::vector<Var> memory) {
    memory.push_back(Var(this->var1, 0));
}

String ProcessInstructions::runAdd(std::vector<Var> memory) {
    String log = "Added ";
    switch (this->instruction_variation) {
        case 0: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);
            memory[i].value = memory[j].value + memory[k].value;
            log += this->var2 + " and " + this->var3 + ". Results stored in " + this->var1;
            return log;
        }
        case 1: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            memory[i].value = memory[j].value + this->constant1;
            log += this->var2 + " and " + std::to_string(this->constant1) + ". Results stored in " + this->var1;
            return log;
        }
        case 2: {
            int i = findVar(this->var1, memory);
            memory[i].value = this->constant1 + this->constant2;
            log += std::to_string(this->constant1) + " and " + std::to_string(this->constant2) + ". Results stored in " + this->var1;
            return log;
        }
    }
    return "Invalid ADD variation.";
}

String ProcessInstructions::runSubtract(std::vector<Var> memory) {
    String log = "Subtracted ";
    switch (this->instruction_variation) {
        case 0: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);
            memory[i].value = memory[j].value - memory[k].value;
            log += this->var2 + " from " + this->var3 + ". Results stored in " + this->var1;
            return log;
        }
        case 1: {
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            memory[i].value = memory[j].value - this->constant1;
            log += this->var2 + " from " + std::to_string(this->constant1) + ". Results stored in " + this->var1;
            return log;
        }
        case 2: {
            int i = findVar(this->var1, memory);
            memory[i].value = this->constant1 - this->constant2;
            log += std::to_string(this->constant1) + " and " + std::to_string(this->constant2) + ". Results stored in " + this->var1;
            return log;
        }
    }
    return "Invalid SUBTRACT variation.";
}

void runSleep() {}
