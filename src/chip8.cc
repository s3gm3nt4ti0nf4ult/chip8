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


bool vm::run_cycle(){
    //TODO
    //Get opcode from memory
    //get pointer to instruction opcode
    //execute function & return 1 || 0 depending on success
     
    return true;
}

bool vm::run(string& fname){
      
    bool quit = false;
    if (this->debuged)
        printf("Running debugged mode.\n");
    
    if (!this->load(fname))
        return false;

    while (!quit){
        while( SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (!run_cycle()){
                quit = false;
            }
        }

    }
    return true;
}

bool vm::load(string& fname){
    std::streampos fsize = 0;
    if (this->debuged)
        printf("Loading binary %s...", fname.c_str());
    
    std::ifstream input_binary(fname.c_str(), std::ios::in | std::ios::binary);

    if (!input_binary.is_open())
        return false;
    fsize = input_binary.tellg();
    input_binary.seekg(0, std::ios::end);
    fsize = input_binary.tellg() - fsize;
    
    input_binary.seekg(0, std::ios::beg);

    if (fsize >= MEMORY_SIZE - 512)
        return false;
   
    input_binary.read(reinterpret_cast<char*>(this->memory), fsize);

    return true;
}

uint16_t vm::getopcode(uint16_t instr, uint8_t pos, uint8_t len){
    uint16_t mask = 0;
    mask = bitmask<uint16_t>(len);
    mask = mask << pos;
    return (instr & mask) >> pos;
}

void vm::(*getinstr())(const uint16_t& params){ //TODO
    return func;
}

bool vm::is_pressed(){
    //TODO implement this with SDL
    return true;
}


bool vm::check_pressed(int8_t &value){
    value = 2;
    if (value)
        return true;
    return false;
}

uint8_t vm::what_pressed(){
    //TODO implement this with SDL
    return 0xFF;
}

void vm::op0NNN(const uint16_t& params){ // unsupportted instruction
    //this func should be empty.... or rise an exception
    throw 10; // TODO add exceptions
}


void vm::op00E0(const uint16_t& params){ // cls instr
    //clear screen TODO
    memset(screen, 0, 32*sizeof(uint64_t));
}


void vm::op00EE(const uint16_t& params){ // ret instr
    if (stack.size() > 0){
        this->PC = this->stack.back();
        this->stack.pop_back();
    }
    else{
       throw 10; 
    }
}

void vm::op1NNN(const uint16_t& params){ // jump instr
    uint16_t addr = getopcode(params, 0, 12);
    if (addr < 4096){ 
        this->PC = params;
    }
    else{
        throw 10;
    }
}

void vm::op2NNN(const uint16_t& params){ // call instr
    uint16_t addr = getopcode(params, 0, 12);
    if (this->stack.size() < 64 &&  addr < 4096){
        this-> PC = addr;
    }
    else{
        throw 10;
    }
}

void vm::op3XKK(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] == kk){
        this->PC += 2;
    }
}


void vm::op4XKK(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] != kk){
        this->PC += 2;
    }
}


void vm::op5XY0(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if(this->V[x] == this->V[y]){
        this->PC += 1;
    }
}

void vm::op6XKK(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    if(this->V[x] == kk){
        this->PC += 1;
    }
}

void vm::op7XKK(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    this->V[x] = this->V[x] + uint8_t(kk);
}

void vm::op8XY0(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[y];
}

void vm::op8XY1(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] | this->V[y];
}

void vm::op8XY2(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] & this->V[y];
}

void vm::op8XY3(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    this->V[x] = this->V[x] ^ this->V[y];
}
 // TODO check VF register!!!
void vm::op8XY4(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    int16_t tmp = int16_t(V[x]) + int16_t(V[y]);
    if (((bitmask<uint16_t>(8) << 8) & tmp) == 0){
        this->V[x] = this->V[x] + this->V[y];
        V[15] = V[15] & (bitmask<uint8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] + this->V[y];
        V[15] = V[15] ^ (bitmask<uint8_t>(8));
    }
}

void vm::op8XY5(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    int16_t tmp = int16_t(V[x]) - int16_t(V[y]);
    if (((bitmask<uint16_t>(8) << 8) & tmp) == 0){
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] & (bitmask<uint8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] ^ (bitmask<uint8_t>(1));
    }
}

void vm::op8XY6(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (((bitmask<uint8_t>(1) ) & V[x]) != 0){
        this->V[x] = this->V[x] >> this->V[y];
        V[15] = V[15] & (bitmask<uint8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] >> this->V[y];
        V[15] = V[15] ^ (bitmask<uint8_t>(8));
    }
}


void vm::op8XY7(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (V[x] <= V[y]){
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] & (bitmask<uint8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] - this->V[y];
        V[15] = V[15] ^ (bitmask<uint8_t>(8));
    }
}

void vm::op8XYE(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (((bitmask<uint8_t>(1) << 8 ) & V[x]) != 0){
        this->V[x] = this->V[x] << this->V[y];
        V[15] = V[15] & (bitmask<uint8_t>(7) << 1);
    }
    else{
        this->V[x] = this->V[x] << this->V[y];
        V[15] = V[15] ^ (bitmask<uint8_t>(8));
    }
}

void vm::op9XY0(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t y = getopcode(params, 0, 4);
    if (V[x] != V[y]){
        this->PC += 1;
    }
}

void vm::opANNN(const uint16_t& params){
    uint16_t value = getopcode(params, 0, 12);
    this->I = value;
}

void vm::opBNNN(const uint16_t& params){
    uint16_t offset = getopcode(params, 0, 12);
    if (offset + V[0] < MEMORY_SIZE){
        this-> PC += V[0];
    }
}

void vm::opCXKK(const uint16_t& params){
    uint16_t x = getopcode(params, 8 ,4);
    uint16_t kk = getopcode(params, 0, 8);
    V[x] = rand()%255 & kk;   
}

void vm::opDXYN(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    uint16_t y = getopcode(params, 4, 4);
    uint16_t n = getopcode(params, 0, 4);
    vector<int8_t> bytes;
    if (this-> I + n < 4096){
        for(int i = this->I; i < n; i++)
            bytes.push_back(memory[i]);
        printf("%d %d", x, y); //TODO implement with display
    }
    else{
        throw 10;
    }
}


void vm::opEX9E(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if(check_pressed(V[x])){
        PC++;
    }
}

void vm::opEXA1(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if(!check_pressed(V[x])){
        PC++;
    }
}


void vm::opFX07(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    V[x] = delay_timer;
}

void vm::opFX0A(const uint16_t& params){ // TODO wait on event
    uint16_t x = getopcode(params, 8, 4);
    while(true){
        if (vm::is_pressed()){
            V[x] = vm::what_pressed();
            break;
        }
    }
}


void vm::opFX15(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16) 
        delay_timer = V[x];
    else{
        throw 10;
    }
}

void vm::opFX18(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16){
        sound_timer = V[x];
    }
    else{
        throw 10;
    }
}


void vm::opFX1E(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16){
        I += V[x];
    }
    else{
        throw 10;
    }
}

void vm::opFX29(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16){
        // TODO
    }
    else{
        throw 10;
    }
}


void vm::opFX33(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16 && I-3 < 4096){
        memory[I] = V[x] / 100;
        memory[I+1] = (V[x] % 100) / 10;
        memory[I+2] = ((V[x] % 100) % 10);
     }
    else{
        throw 10;
    }
}

void vm::opFX55(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16){
        if ( I + x < 4096){
            for(auto i = 0; i < x; i++){
                memory[I+i] = V[i];
            }
        }
        else{
            throw 10;
        }
    }
    else{
        throw 10;
    }
}

void vm::opFX65(const uint16_t& params){
    uint16_t x = getopcode(params, 8, 4);
    if (x < 16){
        if ( I + x < 4096){
            for(auto i = 0; i < x; i++){
                V[i] = memory[I+i] ;
            }
        }
        else{
            throw 10;
        }
    }
    else{
        throw 10;
    }
}

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
