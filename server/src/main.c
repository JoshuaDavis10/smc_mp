#include "logger.h"

#include "game_core.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void choose_positions(int* p1, int* p2) {
    srand(time(NULL));
    *p1 = rand() % NUM_POSITIONS;
    *p2 = rand() % NUM_POSITIONS;
    if(*p1 == *p2) {
        if(*p1 != 0) {
            (*p1)--;
        }
        else {
            (*p1)++;
        }
    }
}

//TODO: fix dis
uint process_command(char* command) {
    LOGINFO("GAME: you entered: %s", command);
    if(strcmp(command, "wl")) {
        if(!gc_walk(DIR_LEFT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "wr")) {
        if(!gc_walk(DIR_RIGHT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "jl")) {
        if(!gc_jump(DIR_LEFT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "jr")) {
        if(!gc_jump(DIR_RIGHT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl1")) {
        if(!gc_roll(DIR_LEFT, 1)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl2")) {
        if(!gc_roll(DIR_LEFT, 2)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl3")) {
        if(!gc_roll(DIR_LEFT, 3)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr1")) {
        if(!gc_roll(DIR_RIGHT, 1)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr2")) {
        if(!gc_roll(DIR_RIGHT, 2)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr3")) {
        if(!gc_roll(DIR_RIGHT, 3)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "0")) {
        if(!gc_use_weapon(0)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "1")) {
        if(!gc_use_weapon(1)) {
            return false;
        }
        return true;
    }
    return false;
}

//TODO: should this game loop realistically just be a function in game_core.c?
int main() {

    //randomly choose starting positions
    int pos1;
    int pos2;
    choose_positions(&pos1, &pos2);
    //randomly choose starting positions

    //initialize
    if(!gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", pos1, pos2)) {
        LOGERROR("failed to initialize game core");
        return -1;
    }
    //initialize

    //game loop
    while(1) {

        mech p1 = gc_get_mech(P1);
        mech p2 = gc_get_mech(P2);
        mech player;
        mech enemy;
        int turn = gc_get_turn();
        if(turn == P1) {
            player = p1;
            enemy = p2;
        }
        if(turn == P2) {
            player = p2;
            enemy = p1;
        }

        LOGINFO("GAME: %s's turn", player.name);
        LOGINFO("GAME: %s pos:    %d                  | %s pos:    %d", player.name, player.position, enemy.name, enemy.position);
        LOGINFO("GAME: %s health: %d                  | %s health: %d", player.name, player.health, enemy.name, enemy.health);
        LOGINFO("GAME: %s weapons:                    | %s weapons:  ", player.name, enemy.name);
        LOGINFO("GAME:     0: range = %d, uses = %d   |     0: range = %d, uses = %d ", player.weapons[0].range, player.weapons[0].uses,
            enemy.weapons[0].range, enemy.weapons[0].uses);
        LOGINFO("GAME:     1: range = %d, uses = %d   |     1: range = %d, uses = %d ", player.weapons[1].range, player.weapons[1].uses,
            enemy.weapons[1].range, enemy.weapons[1].uses);

        int result = 0;
        while(result == 0) {
            LOGINFO("GAME: enter your command: ");
            char command[10];
            fgets(command, 10, stdin);
            if((result = process_command(command)) == 0) {
                LOGWARN("GAME: invalid command: %s", command);
            }
        }
    }
    //game loop

    return 0;
}
