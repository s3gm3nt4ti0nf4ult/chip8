#include "chip8.h"


void vm::initialize(bool if_dbg){
    debuged = if_dbg;
    if (debuged)
        printf("Debuged mode ON");
    else
        printf("Debuged mode OFF");

    sound_timer = 60;
    delay_timer = 60;
    PC = 0;
    I = 0;
    for (auto& i : memory)
        i = 0;
    for (auto& i : V)
        i = 0;
    for (auto& i : screen)
        i = 0;
    if (stack.size() > 0)
        stack.empty();
}

bool vm::run(string& fname){
    if (debuged)
        printf("Running debugged mode.\n");
    load(fname);
    return true;
}

bool vm::load(string& fname){
    if (debuged)
        printf("Loading binary %s...", fname.c_str());
    return true;
}
