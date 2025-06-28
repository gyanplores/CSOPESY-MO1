#include "ProcessInstructions.h"

#include <string>
#include <vector>

using String = std::string;




String ProcessInstructions::runInstruction(std::vector<Var> memory){
    if (this->instruction_type == "PRINT"){
        return runPrint(memory);
    }
    else if (this->instruction_type == "DECLARE"){
        return runDeclare(memory);
    }
}

int ProcessInstructions::findVar(String varname, std::vector<Var> memory){
    for (int i = 0; i < memory.size(); i++) {
    
        if (memory[i].name == varname) {
            return i;
        }
    }

    return findVar(varname, memory);

}

String ProcessInstructions::runPrint(std::vector<Var> memory){
    switch(this->instruction_variation) {
        case 0: //no variable
            return this->constant_string;
            break;
        case 1: //variable infornt of message
            return this->constant_string.append(std::to_string(this->constant1));
            break;
        case 2: //variable behind message
            return std::to_string(this->constant1).append(this->constant_string);
            break;
    }
}

String ProcessInstructions::runDeclare(std::vector<Var> memory){
    memory.push_back(Var(this->var1, this->constant1));
    String log = "Variable ";
    log.append(this->var1);
    log.append(" declared with value ");
    log.append(std::to_string(this->constant1));
    return log;
}

void ProcessInstructions::runDeclareBlank(std::vector<Var> memory){
    memory.push_back(Var(this->var1, 0));
}

String ProcessInstructions::runAdd(std::vector<Var> memory){
    String log = "Added ";
    switch(this->instruction_variation) {
        case 0: //2 variables added

            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);

            memory[i].value = memory[j].value + memory[k].value;

            
            log.append(this->var2);
            log.append(" and ");
            log.append(this->var3);
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
            break;
        case 1: //variable and constant
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);

            memory[i].value = memory[j].value + this->constant1;
            

            log.append(this->var2);
            log.append(" and ");
            log.append(std::to_string(this->constant1));
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
        case 2: //2 constants
            int i = findVar(this->var1, memory);

            memory[i].value = this->constant1 + this->constant2;
            
            log.append(std::to_string(this->constant1));
            log.append(" and ");
            log.append(std::to_string(this->constant2));
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
    }
}


String ProcessInstructions::runSubtract(std::vector<Var> memory){
    String log = "Subtracted ";
    switch(this->instruction_variation) {
        case 0: //2 variables added

            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);
            int k = findVar(this->var3, memory);

            memory[i].value = memory[j].value - memory[k].value;

            
            log.append(this->var2);
            log.append(" from ");
            log.append(this->var3);
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
            break;
        case 1: //variable and constant
            int i = findVar(this->var1, memory);
            int j = findVar(this->var2, memory);

            memory[i].value = memory[j].value - this->constant1;
            

            log.append(this->var2);
            log.append(" from ");
            log.append(std::to_string(this->constant1));
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
        case 2: //2 constants
            int i = findVar(this->var1, memory);

            memory[i].value = this->constant1 - this->constant2;
            
            log.append(std::to_string(this->constant1));
            log.append(" and ");
            log.append(std::to_string(this->constant2));
            log.append(". Results stored in ");
            log.append(this->var1);
            return log;
    }
}

std::vector<ProcessInstructions> ProcessInstructions::processForLoop(std::vector<ProcessInstructions> block, int num ){
    if (block[0].instruction_type == "FOR"){
        block = processForLoop(block, block[0].constant1);

    }
    else{
        for (int i = 0; i < block[0].constant1; i++){
            block.push_back(ProcessInstructions());
            block[block.size()].instruction_type = block[0].constant_string;
        }
    }
    block.erase(block.begin());
    return block;
}

void runSleep();