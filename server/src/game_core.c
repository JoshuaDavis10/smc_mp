#include "game_core.h"

#include "mech.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define WALK_DISTANCE 1
#define JUMP_DISTANCE 2
#define MAX_ROLL_DISTANCE 3

//enum
//for mechfile procesing (MFM is mech file mode... pretty cool huh?)
enum {
    MFM_NONE,
    MFM_NAME,
    MFM_WEAPON,
    MFM_HEALTH,
    MFM_MOVEMENT
};
//enum

static game_core* gc = 0; //game core state pointer

//private functions forward declare
uint switch_turn();
uint dec_actions();
uint load_mech(mech* m, const char* filepath);
//private functions forward declare

//public functions
uint gc_initialize(const char* filepath1, const char* filepath2, int pos1, int pos2) {
    if(gc != 0) {
        LOGWARN("gc_initialize: tried to initialize when alread initialized.");
        return false;
    }

    LOGINFO("initializing game core...");

    gc = (game_core*)malloc(sizeof(game_core));
    memset(gc, 0, sizeof(game_core));
    gc->player = P1;
    gc->enemy = P2;
    gc->actions_left = 1;

    gc->players[P1] = (mech*)malloc(sizeof(mech));
    gc->players[P2] = (mech*)malloc(sizeof(mech));
    memset(gc->players[P1], 0, sizeof(mech));
    memset(gc->players[P2], 0, sizeof(mech));
    if(!load_mech(gc->players[P1], filepath1)) {
        LOGWARN("gc_initialize: failed to initialize. could not load P1 mech from file: '%s'", filepath1);
        return false;
    }
    if(!load_mech(gc->players[P2], filepath2)) {
        LOGWARN("gc_initialize: failed to initialize. could not load P2 mech from file: '%s'", filepath2);
        return false;
    }

    gc->players[P1]->position = pos1;
    gc->players[P2]->position = pos2;

    LOGINFO("P1 position: %d", gc->players[P1]->position);
    LOGINFO("P2 position: %d", gc->players[P2]->position);
    
    LOGINFO("game core initialized.");
    return true;
}

uint gc_shutdown() {
    if(gc == 0) {
        LOGWARN("gc_shutdown: game core has not yet been initialized... cannot shutdown... return false");
        return false;
    }
    free(gc->players[P1]);
    free(gc->players[P2]);
    free(gc);
    LOGINFO("game core has been shutdown");
    return true;
}

uint gc_walk(int dir) {

    mech* player = gc->players[gc->player];
    int p_pos = player->position;
    int e_pos = gc->players[gc->enemy]->position;
    int mult = 1;
    if(dir == DIR_LEFT)
        mult = -1;

    //if enemy is between player and where player is trying to walk
    if(e_pos < p_pos && e_pos >= p_pos + mult*WALK_DISTANCE) {
        LOGWARN("gc_walk: could not execute. enemy (pos: %d) is between player pos: %d and location player is trying to walk to: %d or enemy is on location player is trying to walk to", e_pos, p_pos, p_pos+mult*WALK_DISTANCE);
        return false;
    }

    if(!move(player, dir, WALK_DISTANCE)) {
        LOGWARN("gc_walk: could not execute. see output from 'move' function above ^");
        return false;
    }
    
    if(dir == DIR_LEFT) {
        LOGINFO("gc_walk: %s walked left", player->name);
        dec_actions();
        return true;
    }
    if(dir == DIR_RIGHT) {
        LOGINFO("gc_walk: %s walked right", player->name);
        dec_actions();
        return true;
    }

    LOGWARN("gc_walk: invalid param value for 'dir': %d (should be %d (left) or %d (right))", dir, DIR_LEFT, DIR_RIGHT);
    return false;
}

uint gc_jump(int dir) {
    mech* player = gc->players[gc->player];
    int p_pos = player->position;
    int e_pos = gc->players[gc->enemy]->position;
    int mult = 1;
    if(dir == DIR_LEFT)
        mult = -1;

    //if enemy is on the spot player is trying to jump to
    if(e_pos == p_pos+mult*JUMP_DISTANCE) {
        LOGWARN("gc_jump: could not execute. enemy (pos: %d) is on pos player is trying to jump to: %d", e_pos, p_pos+mult*JUMP_DISTANCE);
        return false;
    }

    if(!move(player, dir, JUMP_DISTANCE)) {
        LOGWARN("gc_jump: could not execute. see output from 'move' function above ^");
        return false;
    }
    
    if(dir == DIR_LEFT) {
        LOGINFO("gc_jump: %s jumped left", player->name);
        dec_actions();
        return true;
    }
    if(dir == DIR_RIGHT) {
        LOGINFO("gc_jump: %s jumped right", player->name);
        dec_actions();
        return true;
    }

    LOGWARN("gc_jump: invalid param value for 'dir': %d (should be %d (left) or %d (right))", dir, DIR_LEFT, DIR_RIGHT);
    return false;
}

uint gc_roll(int dir, int dist) {
    mech* player = gc->players[gc->player];
    int p_pos = player->position;
    int e_pos = gc->players[gc->enemy]->position;
    int mult = 1;
    if(dir == DIR_LEFT)
        mult = -1;

    if(dist <= 0 || dist > MAX_ROLL_DISTANCE) {
        LOGWARN("gc_roll: invalid param value for 'dist': %d (should be between 1 and %d, inclusive)", dist, MAX_ROLL_DISTANCE);
        return false;
    }

    //if enemy is between player and where player is trying to walk
    if(e_pos < p_pos && e_pos >= p_pos + mult*dist) {
        LOGWARN("gc_roll: could not execute. enemy (pos: %d) is between player pos: %d and location player is trying to roll to: %d or enemy is on location player is trying to roll to", e_pos, p_pos, p_pos+mult*dist);
        return false;
    }

    if(!move(player, dir, dist)) {
        LOGWARN("gc_roll: could not execute. see output from 'move' function above ^");
        return false;
    }
    
    if(dir == DIR_LEFT) {
        LOGINFO("gc_roll: %s rolled left %d spaces", player->name, dist);
        dec_actions();
        return true;
    }
    if(dir == DIR_RIGHT) {
        LOGINFO("gc_roll: %s rolled right %d spaces", player->name, dist);
        dec_actions();
        return true;
    }

    LOGWARN("gc_roll: invalid param value for 'dir': %d (should be %d (left) or %d (right))", dir, DIR_LEFT, DIR_RIGHT);
    return false;
}

uint gc_use_weapon(int w_num) {
    if(w_num < 0 || w_num >= NUM_WEAPONS) {
        LOGWARN("gc_use_weapon: invalid param value for 'w_num': %d (should be between 0 and %d, inclusive", w_num, NUM_WEAPONS-1);
        return false;
    }

    mech* player = gc->players[gc->player];
    mech* enemy  = gc->players[gc->enemy];
    weapon weap = player->weapons[w_num];

    if(player->weapons[w_num].uses <= 0) {
        LOGWARN("gc_use_weapon: cannot use weapon %d for mech: %s since it is out of uses", w_num, player->name);
        return false;
    }

    int p_pos = player->position;
    int e_pos = enemy->position;
    int dist = e_pos - p_pos;
    if(dist < 0) {
        dist = -dist;
    }
    if(dist > player->weapons[w_num].range) {
        LOGWARN("gc_use_weapon: cannot use %s's weapon %d: out of range", player->name, w_num);
        return false;
    }

    if(!take_damage(enemy, weap.damage)) {
        LOGWARN("gc_use_weapon: failed to damage %s. see output from 'take_damage' function above ^", enemy->name);
        return false;
    }

    if(!use_weapon(player, w_num)) {
        LOGWARN("gc_use_weapon: failed to use weapon %d for mech: %s. see output from 'use_weapon' function above ^", w_num, player->name);
        return false;
    }

    dec_actions();

    LOGINFO("gc_use_weapon: %s used weapon %d successfully", player->name, w_num);
    return true;
}

mech gc_get_mech(int p_num) {
    if(p_num != P1 && p_num != P2) {
        LOGWARN("gc_get_mech: invalid param value for 'p_num': %d (should be %d (P1) or %d (P2))", p_num, P1, P2);
    }
    return *(gc->players[p_num]);
}
//public functions

//private functions
uint switch_turn() {
    int turn = gc->player;
    if(turn == P1) {
        gc->player = P2;
        gc->enemy = P1;
        LOGINFO("switch_turn: switching turn to P2");
        return true;
    }
    if(turn == P1) {
        gc->player = P1;
        gc->enemy = P2;
        LOGINFO("switch_turn: switching turn to P1");
        return true;
    }
    LOGWARN("switch_turn: couldn't switch turn. game core's value for 'player' is: %d, which is invalid", turn);
    return false;
}

uint dec_actions() {
    gc->actions_left-=1;
    if(gc->actions_left == 0) {
        if(!switch_turn()) 
            return false;
        gc->actions_left = 2;
        return true;
    }
}

uint load_mech(mech* m, const char* filepath) {

    FILE* m_file = fopen(filepath, "r+");

    if(m_file == NULL) {
        LOGWARN("load_mech: failed to open file: '%s'", filepath);
        return false;
    }

    //while haven't reached end of file read a line... process that line
    char buf[1000];
    char temp[1000];
    int fp_state = MFM_NONE; //fp is 'file process'... pretty cool huh?
    int weap_count = 0; 
    int weap_sect_line = 0;
    weapon w[NUM_WEAPONS];
    int len;
    while(fgets(buf, 1000, m_file) != NULL) {
        strcpy(temp, buf);
        if(strcmp(buf, "\n") == 0 && fp_state != MFM_NONE) {
            fp_state = MFM_NONE;
        }

        //none state
        if(fp_state == MFM_NONE) {
            if(strcmp(buf, "#name\n") == 0) {
                fp_state = MFM_NAME;
            }
            if(strcmp(buf, "#weapon\n") == 0) {
                fp_state = MFM_WEAPON;
                weap_count++;
                if(weap_count > NUM_WEAPONS) {
                    LOGWARN("load_mech: mechfile has %d weapons so far. should only have %d weapons", weap_count, NUM_WEAPONS);
                    return false;
                }
            }
            if(strcmp(buf, "#health\n") == 0) {
                fp_state = MFM_HEALTH;
            }
            if(strcmp(buf, "#movement\n") == 0) {
                fp_state = MFM_MOVEMENT;
            }
        }
        //name state
        else if(fp_state == MFM_NAME) {
            len = strlen(buf);
            buf[len-1] = '\0'; //should get rid of \n?
            strcpy(m->name, buf);            
        }
        //health state
        else if(fp_state == MFM_HEALTH) {
            len = strlen(buf);
            buf[len-1] = '\0'; //should get rid of \n?
            m->health = atoi(buf);
        }
        //movement state
        else if(fp_state == MFM_MOVEMENT) {
            if(strcmp(buf, "roll\n") == 0) {
                m->can_jump = 0;
                m->can_roll = 1;
            }
            else if(strcmp(buf, "jump\n") == 0) {
                m->can_jump = 1;
                m->can_roll = 0;

            }
            else if(strcmp(buf, "walk\n") == 0) {
                m->can_jump = 0;
                m->can_roll = 0;
            }
            else {
                LOGWARN("load_mech: invalid #movement section option: %s. should be 'roll', 'walk', or 'jump'.", buf);
                return false;
            }

        }
        //weapon state
        else if(fp_state == MFM_WEAPON) {
            switch(weap_sect_line) {
                case 0:
                    len = strlen(buf);
                    buf[len-1] = '\0'; //should get rid of \n?
                    w[weap_count-1].damage = atoi(buf);
                    weap_sect_line++;
                    break;
                case 1:
                    len = strlen(buf);
                    buf[len-1] = '\0'; //should get rid of \n?
                    w[weap_count-1].uses = atoi(buf);
                    weap_sect_line++;
                    break;
                case 2:
                    len = strlen(buf);
                    buf[len-1] = '\0'; //should get rid of \n?
                    w[weap_count-1].range = atoi(buf);
                    weap_sect_line++;
                    break;
                case 3:
                    len = strlen(buf);
                    buf[len-1] = '\0'; //should get rid of \n?
                    strcpy(w[weap_count-1].name, buf);
                    weap_sect_line = 0;
                    break;
            }
        }

        for(int i = 0; i < NUM_WEAPONS; i++) {
            m->weapons[i] = w[i];
        }

        //TODO: temp... print all the mech info

        //TEMP
        printf("%s", temp);
        //TEMP
    }

    LOGINFO("mech:\nname: %s\nhealth: %d\nmovement:\n - roll: %d\n - jump: %d\nweapons:\n - weapon 1:\n - > damage: %d\n - > uses: %d\n - > range: %d\n - > name: %s\n - weapon 2:\n - > damage: %d\n - > uses: %d\n - > range: %d\n - > name: %s\n", m->name, m->health, m->can_roll, m->can_jump, m->weapons[0].damage, m->weapons[0].uses, m->weapons[0].range, m->weapons[0].name, m->weapons[1].damage, m->weapons[1].uses, m->weapons[1].range, m->weapons[1].name);  

    return true;
}
//private functions
