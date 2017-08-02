#include "chip8.h"


void vm::initialize(bool if_dbg){
    this->debuged = if_dbg;
    if (debuged)
        printf("Debuged mode ON");
    else
        printf("Debuged mode OFF");

    this->sound_timer = 60;
    this->delay_timer = 60;
    this->PC = 0;
    this->I = 0;
    for (auto& i : this->memory)
        i = 0;
    for (auto& i : this->V)
        i = 0;
    for (auto& i : this->screen)
        i = 0;
    if (this->stack.size() > 0)
        this->stack.empty();
}

bool vm::run(string& fname){
    if (this->debuged)
        printf("Running debugged mode.\n");
    load(fname);

    while (true){
        vm::run_cycle();
    }
    return true;
}

bool vm::load(string& fname){
    if (this->debuged)
        printf("Loading binary %s...", fname.c_str());
    return true;
}

uint16_t vm::getopcode(uint16_t instr, uint8_t pos, uint8_t len){
    uint16_t mask = 0;
    mask = BIT_MASK(uint16_t, len);
    mask = mask >> pos;
    return (instr & mask) << pos;
}


void vm::op0NNN(){ // unsupportted instruction
    //this func should be empty.... or rise exception
    throw 10;
}


void vm::op00E0(){ // cls instr
    //clear screen TODO
}


void vm::op00EE(){ // ret instr
    if (stack.size() > 0){
        this->PC = this->stack.back();
        this->stack.pop_back();
    }
    else{
       throw 10; 
    }
}

void vm::1NNN(uint16_t params){ // jump instr
    uint16_t addr = getopcode(params, 4, 12);
    if (addr > 0 && addr < 4096){ 
        this->PC = params;
    }
    else{
        throw 10;
    }
}

void vm::2NNN(uint16_t params){ // call instr
    uint16_t addr = getopcode(params, 4, 12);
    if (this->stack.size() < 64 && addr >= 0 && addr < 4096){
        this-> PC = addr;
    }
    else{
        throw 10;
    }
}




