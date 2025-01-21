#pragma once

#include "defines.h"

#define NUM_WEAPONS 2
#define MAX_NAME_LENGTH 100

//structs
typedef struct {
    int damage;
    int uses;
    int range;
    char name[MAX_NAME_LENGTH];
} weapon;

typedef struct {
    int position;
    int health;
    uint can_jump;
    uint can_roll;
    weapon weapons[NUM_WEAPONS];
    char name[MAX_NAME_LENGTH];
} mech;
//structs

//functions
uint use_weapon(mech* mech, int w_num);
uint take_damage(mech* mech, int dmg);
uint move(mech* mech, int dir, int dist);
//functions
