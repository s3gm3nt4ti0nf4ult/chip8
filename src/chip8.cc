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
        //vm::run_cycle(); TODO: cycle, refresh, counterUpdate, events
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
    mask = bitmask<uint16_t>(len);
    mask = mask << pos;
    return (instr & mask) >> pos;
}


void vm::op0NNN(){ // unsupportted instruction
    //this func should be empty.... or rise exception
    throw 10; // TODO add exceptions
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

void vm::op1NNN(uint16_t params){ // jump instr
    uint16_t addr = getopcode(params, 0, 12);
    if (addr < 4096){ 
        this->PC = params;
    }
    else{
        throw 10;
    }
}

void vm::op2NNN(uint16_t params){ // call instr
    uint16_t addr = getopcode(params, 0, 12);
    if (this->stack.size() < 64 &&  addr < 4096){
        this-> PC = addr;
    }
    else{
        throw 10;
    }
}

void vm::op3XKK(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] == kk){
        this->PC += 2;
    }
}


void vm::op4XKK(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] != kk){
        this->PC += 2;
    }
}


void vm::op5XY0(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if(this->V[x] == this->V[y]){
        this->PC += 1;
    }
}

void vm::op6XKK(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] == this->V[y]){
        this->PC += 1;
    }
}

void vm::op7XKK(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    this->V[x] = this->V[x] + uint8_t(kk);
}

void vm::op8xy0(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[y];
}

void vm::op8XY1(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] | this->V[y];
}

void vm::op8XY2(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] & this->V[y];
}

void vm::op8XY3(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] ^ this->V[y];
}
 // TODO check VF register!!!
void vm::op8XY4(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    int16_t tmp = int16_t(V[x]) + int16_t(V[y]);
    if (((bitmask<int16_t>(8) << 8) & tmp) == 0){
        this->V[x] = this->V[x] + this->V[y];
        V[15] = V[15] & (bitmask<int8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] + this->V[y];
        V[15] = V[15] ^ (bitmask<int8_t>(8));
    }
}

void vm::op8XY5(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    int16_t tmp = int16_t(V[x]) - int16_t(V[y]);
    if (((bitmask<int16_t>(8) << 8) & tmp) == 0){
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] & (bitmask<int8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] ^ (bitmask<int8_t>(1));
    }
}

void vm::op8XY6(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (((bitmask<int8_t>(1) ) & V[x]) != 0){
        this->V[x] = this->V[x] >> this->V[y];
        V[15] = V[15] & (bitmask<int8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] >> this->V[y];
        V[15] = V[15] ^ (bitmask<int8_t>(8));
    }
}


void vm::op8XY7(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (V[x] <= V[y]){
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] & (bitmask<int8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] ^ (bitmask<int8_t>(8));
    }
}

void vm::op8XYE(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (((bitmask<int8_t>(1) << 8 ) & V[x]) != 0){
        this->V[x] = this->V[x] << this->V[y];
        v[15] = v[15] & (bitmask<int8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] << this->V[y];
        v[15] = v[15] ^ (bitmask<int8_t>(8));
    }
}

void vm::op9XY0(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (V[x] != V[y]){
        this->PC += 1;
    }
}

void vm::opANNN(uint16_t params){
    uint16_t value = getopcode(params, 0, 12);
    this->I = value;
}

void vm::opBNNN(uint16_t params){
    uint16_t offset = getopcode(params, 0, 12);
    this-> PC += V[0];
}

void vm::opCXKK(uint16_t params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    V[x] = random()%255 & kk;   
}

void vm::opDXYN(uint16_t params){
    uint16_t x = getopcode(params, 8, 4);
    uint16_t y = getopcode(params, 4, 4);
    uint16_t n = getopcode(params, 0, 4);
    vector<int8_t> bytes;
    if (this-> I + i < 4096){
        for(int i = this->I; i < n; i++)
            bytes.push_back(memory[i]);
    }
    else{
        throw 10;
    }
}


void vm::opEX9E(uint16_t params){
    uint16_t x = getopcode(params, 8, 4);
    if(vm::check_pressed(V[x])){
        PC++;
    }
}

void vm::opEXA1(uint16_t params){
    uint16_t x = getopcode(params, 8, 4);
    if(!vm::check_pressed(V[x])){
        PC++;
    }
}



