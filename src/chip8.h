#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <climits>
#include <random>
#include "cleanup.h"

using std::vector;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



class vm{
public:
    bool debuged = false;
    uint8_t memory[4096]; //4kB of RAM, first 512 used to store some stuff by vm
    uint16_t I;
    uint16_t PC;
    uint64_t screen[32]; //32x64 screen
    int8_t V[16]; // 16 x 8 bit register
    uint8_t delay_timer, sound_timer;
    std::vector<int16_t> stack;
    void initialize(bool);
    bool run(string&);   
    bool run_cycle();
    

private:
    bool load(string&);
    

    uint16_t getopcode(uint16_t, uint8_t, uint8_t);

    //opcodes
    void op0NNN();
    void op00E0();
    void op00EE();
    void op1NNN(uint16_t);
    void op2NNN(uint16_t);
    void op3XKK(uint16_t);
    void op4XNN(uint16_t);
    void op5XY0(uint16_t);
    void op6XNN(uint16_t);
    void op7XNN(uint16_t);
    void op8XY0(uint16_t);
    void op8XY1(uint16_t);
    void op8XY2(uint16_t);
    void op8XY3(uint16_t);
    void op8XY4(uint16_t);
    void op8XY5(uint16_t);
    void op8XY6(uint16_t);
    void op8XY7(uint16_t);
    void op8XYE(uint16_t);
    void op9XY0(uint16_t);
    void opANNN(uint16_t);
    void opBNNN(uint16_t);
    void opCXNN(uint16_t);
    void opDXYN(uint16_t);
    void opEX9E(uint16_t);
    void opEXA1(uint16_t);
    void opFX07(uint16_t);
    void opFX0A(uint16_t);
    void opFX15(uint16_t);
    void opFX18(uint16_t);
    void opFX1E(uint16_t);
    void opFX29(uint16_t);
    void opFX33(uint16_t);
    void opFX55(uint16_t);
    void opFX65(uint16_t);
    
};

template <typename R>
static constexpr R bitmask(unsigned int const onecount)
{
//  return (onecount != 0)
//      ? (static_cast<R>(-1) >> ((sizeof(R) * CHAR_BIT) - onecount))
//      : 0;
    return static_cast<R>(-(onecount != 0))
        & (static_cast<R>(-1) >> ((sizeof(R) * CHAR_BIT) - onecount));
} // link: https://stackoverflow.com/a/28703383/6849518
//  151413121110 9 8 7 6 5 4 3 2 1 0
//  | | | | | | | | | | | | | | | | |
