#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <climits>
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
    

    // opcodes
    uint16_t getopcode(uint16_t, uint8_t, uint8_t);
    void op0NNN();
    void op00E0();
    void op00EE();
    void op1NNN(uint16_t);
    void op2NNN(uint16_t);
    /*
    void op3XNN();
    void op4XNN();
    void op5XY0();
    void op6XNN();
    void op7XNN();
    void op8XY0();
    void op8XY1();
    void op8XY2();
    void op8XY3();
    void op8XY4();
    void op8XY5();
    void op8XY6();
    void op8XY7();
    void op8XYE();
    void op9XY0();
    void opANNN();
    void opBNNN();
    void opCXNN();
    void opDXYN();
    void opEX9E();
    void opEXA1();
    void opFX07();
    void opFX0A();
    void opFX15();
    void opFX18();
    void opFX1E();
    void opFX29();
    void opFX33();
    void opFX55();
    void opFX65();
    */
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
