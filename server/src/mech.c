#include "mech.h"

#include "logger.h"

uint use_weapon(mech* mech, int w_num) {
    if(w_num >= NUM_WEAPONS || w_num < 0) {
        LOGERROR("use_weapon: invalid param value for 'w_num': %d (should be number between 0 and %d, inclusive)", w_num, NUM_WEAPONS-1);
        return false;
    }
    int temp = mech->weapons[w_num].uses;
    mech->weapons[w_num].uses -= 1;
    LOGINFO("use_weapon: decreased %s's uses for weapon %d from %d to %d", mech->name, w_num, temp, mech->weapons[w_num].uses);
    return true;
}

uint take_damage(mech* mech, int dmg) {
    if(mech->health <= 0) {
        LOGWARN("take_damage: tried to run on mech: %s, whose health is <= 0. returning false.", mech->name);
        return false;
    }
    int temp = mech->health;
    mech->health -= dmg;
    LOGINFO("take_damage: decreased %s's health from %d to %d", temp, mech->health);
    return true;
}

uint move(mech* mech, int dir, int dist) {
    if(dir == DIR_LEFT) {
        if(mech->position - dist < 0) {
            LOGWARN("move: cannot move mech %s left by %d. %s is at position %d", mech->name, dist, mech->name, mech->position);
            return false;
        }
        int temp = mech->position;
        mech->position -= dist;
        LOGINFO("move: moved %s from pos: %d to pos: %d", mech->name, temp, mech->position);
        return true;
    }
    if(dir == DIR_RIGHT) {
        if(mech->position + dist >= NUM_POSITIONS) {
            LOGWARN("move: cannot move mech %s right by %d. %s is at position %d (furthest right position is pos: %d", 
                mech->name, dist, mech->name, mech->position, NUM_POSITIONS-1);
            return false;
        }
        int temp = mech->position;
        mech->position += dist;
        LOGINFO("move: moved %s from pos: %d to pos: %d", mech->name, temp, mech->position);
        return true;

    }
    LOGERROR("move: invalid param value for 'dir': %d (should be %d (left) or %d (right)", dir, DIR_LEFT, DIR_RIGHT);
    return false;
}
