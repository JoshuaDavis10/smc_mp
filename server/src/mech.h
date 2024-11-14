#pragma once

#include "defines.h"

#define NUM_WEAPONS 2
#define MAX_NAME_LENGTH 100

//structs
typedef struct {
    int damage;
    int uses;
    char name[MAX_NAME_LENGTH];
} weapon;

typedef struct {
    uint position;
    int health;
    uint can_jump;
    uint can_roll;
    weapon weapon[NUM_WEAPONS];
    char name[MAX_NAME_LENGTH];
} mech;
//structs

//functions
uint use_weapon(int w_num);
uint take_damage(int dmg);
uint move(int dir, int dist);
//functions
