#pragma once

#include "mech.h"

//structs
typedef struct {
    int enemy;
    int player;
    int actions_left;   
    mech* players[2];
} game_core;
//structs

//functions
uint gc_initialize(const char* filepath1, const char* filepath2, int pos1, int pos2);
uint gc_shutdown();
uint gc_walk(int dir);
uint gc_jump(int dir);
uint gc_roll(int dir, int dist);
uint gc_use_weapon(int w_num);
mech gc_get_mech(int p_num);
//functions
