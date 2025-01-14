#include "logger.h"

#include "game_core.h"
#include "socket.h"

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

uint process_command(char* command) {
    LOGINFO("GAME: you entered: %s", command);
    if(strcmp(command, "wl") == 0) {
        if(!gc_walk(DIR_LEFT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "wr") == 0) {
        if(!gc_walk(DIR_RIGHT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "jl") == 0) {
        if(!gc_jump(DIR_LEFT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "jr") == 0) {
        if(!gc_jump(DIR_RIGHT)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl1") == 0) {
        if(!gc_roll(DIR_LEFT, 1)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl2") == 0) {
        if(!gc_roll(DIR_LEFT, 2)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rl3") == 0) {
        if(!gc_roll(DIR_LEFT, 3)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr1") == 0) {
        if(!gc_roll(DIR_RIGHT, 1)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr2") == 0) {
        if(!gc_roll(DIR_RIGHT, 2)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "rr3") == 0) {
        if(!gc_roll(DIR_RIGHT, 3)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "0") == 0) {
        if(!gc_use_weapon(0)) {
            return false;
        }
        return true;
    }
    if(strcmp(command, "1") == 0) {
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
    if(!srv_initialize()) {
        LOGERROR("failed to initialize server");
        return -1;
    }

    if(!gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", pos1, pos2)) {
        LOGERROR("failed to initialize game core");
        return -1;
    }
    //initialize

    //TODO: game loop in a game_core.c function?
    //game loop
    while(1) {

        mech p1 = gc_get_mech(P1);
        mech p2 = gc_get_mech(P2);


        if(p1.health <= 0) {
            LOGINFO("GAME: player 2 (%s) wins!", p2.name);
            break;
        }
        if(p2.health <= 0) {
            LOGINFO("GAME: player 1 (%s) wins!", p1.name);
            break;
        }

        mech player;
        mech enemy;
        int turn = gc_get_turn();
        int enemy_id;
        if(turn == P1) {
            player = p1;
            enemy = p2;
            enemy_id = P2;
        }
        if(turn == P2) {
            player = p2;
            enemy = p1;
            enemy_id = P1;
        }

        char msg_player[MAX_MESSAGE_SIZE];
        char msg_enemy[MAX_MESSAGE_SIZE];
        char temp[MAX_MESSAGE_SIZE];
        snprintf(msg_player, MAX_MESSAGE_SIZE, "you(P%d):\t\t\t\tenemy:\n\thealth: %d\t\t\thealth: %d\n\tpos: %d\t\t\t\tpos: %d\n\tweapons:\t\t\tweapons:\n\t\t0:\t\t\t\t0:\n\t\t\tdamage: %d\t\t\tdamage: %d\n\t\t\tuses: %d\t\t\t\tuses: %d\n\t\t\trange: %d\t\t\trange: %d\n\t\t1:\t\t\t\t1:\n\t\t\tdamage: %d\t\t\tdamage: %d\n\t\t\tuses: %d\t\t\t\tuses: %d\n\t\t\trange: %d\t\t\trange: %d\n\tjump: %d\t\t\t\tjump: %d\n\troll: %d\t\t\t\troll: %d", 
            turn+1, player.health, 
            enemy.health, player.position, enemy.position, player.weapons[0].damage, enemy.weapons[0].damage, player.weapons[0].uses, 
            enemy.weapons[0].uses, player.weapons[0].range, enemy.weapons[0].range, player.weapons[1].damage, enemy.weapons[1].damage, 
            player.weapons[1].uses, enemy.weapons[1].uses, player.weapons[1].range, enemy.weapons[1].range, player.can_jump, enemy.can_jump,
            player.can_roll, enemy.can_roll);
        snprintf(msg_enemy, MAX_MESSAGE_SIZE, "you(P%d):\t\t\t\tenemy:\n\thealth: %d\t\t\thealth: %d\n\tpos: %d\t\t\t\tpos: %d\n\tweapons:\t\t\tweapons:\n\t\t0:\t\t\t\t0:\n\t\t\tdamage: %d\t\t\tdamage: %d\n\t\t\tuses: %d\t\t\t\tuses: %d\n\t\t\trange: %d\t\t\trange: %d\n\t\t1:\t\t\t\t1:\n\t\t\tdamage: %d\t\t\tdamage: %d\n\t\t\tuses: %d\t\t\t\tuses: %d\n\t\t\trange: %d\t\t\trange: %d\n\tjump: %d\t\t\t\tjump: %d\n\troll: %d\t\t\t\troll: %d", 
            enemy_id+1, enemy.health, 
            player.health, enemy.position, player.position, enemy.weapons[0].damage, player.weapons[0].damage, enemy.weapons[0].uses, 
            player.weapons[0].uses, enemy.weapons[0].range, player.weapons[0].range, enemy.weapons[1].damage, player.weapons[1].damage, 
            enemy.weapons[1].uses, player.weapons[1].uses, enemy.weapons[1].range, player.weapons[1].range, enemy.can_jump, player.can_jump,
            enemy.can_roll, player.can_roll);
        srv_send_msg(msg_player, turn);
        srv_recv_msg(temp, turn);
        srv_send_msg(msg_enemy, enemy_id);
        srv_recv_msg(temp, enemy_id);

        LOGINFO("GAME: %s's turn", player.name);
        LOGINFO("GAME: %s pos:    %d                  | %s pos:    %d", player.name, player.position, enemy.name, enemy.position);
        LOGINFO("GAME: %s health: %d                  | %s health: %d", player.name, player.health, enemy.name, enemy.health);
        LOGINFO("GAME: %s weapons:                    | %s weapons:  ", player.name, enemy.name);
        LOGINFO("GAME:     0: range = %d, uses = %d, damage = %d   |     0: range = %d, uses = %d, damage = %d ", player.weapons[0].range, player.weapons[0].uses, player.weapons[0].damage,
            enemy.weapons[0].range, enemy.weapons[0].uses, enemy.weapons[0].damage);
        LOGINFO("GAME:     1: range = %d, uses = %d, damage = %d   |     1: range = %d, uses = %d, damage = %d ", player.weapons[1].range, player.weapons[1].uses, player.weapons[1].damage,
            enemy.weapons[1].range, enemy.weapons[1].uses, enemy.weapons[1].damage);

        char command[MAX_MESSAGE_SIZE];

        int result = 0;
        while(result == 0) {
            srv_send_msg("A", turn);
            LOGINFO("waiting on command from p%d...", turn);
            srv_recv_msg(command, turn);
            LOGINFO("received command: %s from %d", command, turn);
            if((result = process_command(command)) == 0) {
                LOGWARN("GAME: invalid command: %s", command);
            }
        }
    }
    //game loop

    return 0;
}
