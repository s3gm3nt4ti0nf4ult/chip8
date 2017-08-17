#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <climits>
#include <random>
#include <cstdlib>
#include <ctime>
#include "cleanup.h"


#define MEMORY_SIZE 4096

using std::vector;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



class vm{
public:
    bool debuged = false;
    uint8_t memory[MEMORY_SIZE]; //4kB of RAM, first 512 used to store some stuff by vm
    uint16_t I;
    uint16_t PC;
    uint64_t screen[32]; //32x64 screen
    int8_t V[16]; // 16 x 8 bit register
    uint8_t delay_timer, sound_timer;
    std::vector<int16_t> stack;
    void initialize(bool);
    bool run(string&);   
    SDL_Event e;
 

private:
    bool load(string&);
    bool run_cycle();
    bool is_pressed();
    bool check_pressed(int8_t&);
    uint8_t what_pressed();
    uint16_t getopcode(uint16_t, uint8_t, uint8_t);
    void  (*getinstr())(const uint16_t&);  //TODO implement returning pointer to opcode function

    //opcodes
    void op0NNN(const uint16_t&);
    void op00E0(const uint16_t&);
    void op00EE(const uint16_t&);
    void op1NNN(const uint16_t&);
    void op2NNN(const uint16_t&);
    void op3XKK(const uint16_t&);
    void op4XKK(const uint16_t&);
    void op5XY0(const uint16_t&);
    void op6XKK(const uint16_t&);
    void op7XKK(const uint16_t&);
    void op8XY0(const uint16_t&);
    void op8XY1(const uint16_t&);
    void op8XY2(const uint16_t&);
    void op8XY3(const uint16_t&);
    void op8XY4(const uint16_t&);
    void op8XY5(const uint16_t&);
    void op8XY6(const uint16_t&);
    void op8XY7(const uint16_t&);
    void op8XYE(const uint16_t&);
    void op9XY0(const uint16_t&);
    void opANNN(const uint16_t&);
    void opBNNN(const uint16_t&);
    void opCXKK(const uint16_t&);
    void opDXYN(const uint16_t&);
    void opEX9E(const uint16_t&);
    void opEXA1(const uint16_t&);
    void opFX07(const uint16_t&);
    void opFX0A(const uint16_t&);
    void opFX15(const uint16_t&);
    void opFX18(const uint16_t&);
    void opFX1E(const uint16_t&);
    void opFX29(const uint16_t&);
    void opFX33(const uint16_t&);
    void opFX55(const uint16_t&);
    void opFX65(const uint16_t&);
    
};
//TODO move to another .h file
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
//
void logSDLError(std::ostream, const std::string);
