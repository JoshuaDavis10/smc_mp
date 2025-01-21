#include "logger.h"
#include "files.h"

#include "game_core.h"
#include "socket.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//forward declare private functions
uint get_mechfile(int client);
uint send_mechfile(int client);
uint process_command(char* command);
uint get_line(const char* expected, file mechfile, int client);
uint send_line(int client, file mechfile);
//forward declare private functions

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

//TODO: should this game loop realistically just be a function in game_core.c?
int main() {

    //initialize sockets
    if(!srv_initialize()) {
        LOGERROR("failed to initialize server");
        return -1;
    }
    //initialize sockets

    //get mechfiles
    if(!get_mechfile(P1)) {
        LOGERROR("Failed to get mechfile for player 1. Exiting program.");
        return -1;
    }
    if(!get_mechfile(P2)) {
        LOGERROR("Failed to get mechfile for player 1. Exiting program.");
        return -1;

    }
    //get mechfiles

    //TODO: send each client their opponents mechfiles, so that clients can initialize their own game states
    if(!send_mechfile(P1)) {
        LOGERROR("Failed to send mechfile to player 1.");
        return -1;
    }
    if(!send_mechfile(P2)) {
        LOGERROR("Failed to send mechfile to player 2.");
        return -1;
    }

    //randomly choose starting positions
    int pos1;
    int pos2;
    choose_positions(&pos1, &pos2);
    //randomly choose starting positions

    //initialize game state
    //TODO: initialize game state from the mechfiles we got from clients
    if(!gc_initialize("../server/mechfiles/mech1.txt", "../server/mechfiles/mech2.txt", pos1, pos2)) {
        LOGERROR("failed to initialize game core");
        return -1;
    }
    //initialize game state

    //TODO: after all game states initialized 

    LOGINFO("Not starting game loop... stopping here bc we're just testing if mechfiles are loading correctly");

    //game loop
    /*
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

        
        //TODO: these messages should just be updates
        //they should also just be in the process_command function yeah?
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
        //TODO: these messages should just be updates
        //they should also just be in the process_command function yeah?

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
            //TODO: if command invalid, notify client, otherwise send update to both clients...
            LOGINFO("received command: %s from %d", command, turn);
            if((result = process_command(command)) == 0) {
                LOGWARN("GAME: invalid command: %s", command);
            }
        }
    }
    //game loop
    */

    return 0;
}

uint get_mechfile(int client) {

    file mechfile;
    
    //send request to get mechfile data
    if(!srv_send_msg("MRS", client)) {
        LOGERROR("Failed to request mechfile data from player 1. Exiting program.");
        return false;
    }

    //attempt to receive "ACK"
    char buf[2048];
    if(!srv_recv_msg(buf, client)) {
        LOGERROR("Failed to receive client response. Exiting program.");
        return false;
    }

    if(strcmp(buf, "ACK") == 0) {

        //open mechfile to write to
        if(client == P1) {
            if(!open_file("../server/mechfiles/mech1.txt", &mechfile, "w")) {
                LOGERROR("Failed to open mechfile for player 1.");
                return false;
            }
        }
        else {
            if(!open_file("../server/mechfiles/mech2.txt", &mechfile, "w")) {
                LOGERROR("Failed to open mechfile for player 2.");
                return false;
            }
        }

        if(!get_line("#name", mechfile, client)) {
            LOGERROR("Failed to read line: #name.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("\n", mechfile, client)) {
            LOGERROR("Failed to read line: \\n.");
            return false;
        }
        if(!get_line("#health", mechfile, client)) {
            LOGERROR("Failed to read line: #health.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("\n", mechfile, client)) {
            LOGERROR("Failed to read line: \\n.");
            return false;
        }
        if(!get_line("#movement", mechfile, client)) {
            LOGERROR("Failed to read line: #health.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("\n", mechfile, client)) {
            LOGERROR("Failed to read line: \\n.");
            return false;
        }
        if(!get_line("#weapon", mechfile, client)) {
            LOGERROR("Failed to read line: #health.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("\n", mechfile, client)) {
            LOGERROR("Failed to read line: \\n.");
            return false;
        }
        if(!get_line("#weapon", mechfile, client)) {
            LOGERROR("Failed to read line: #health.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }
        if(!get_line("N/A", mechfile, client)) {
            LOGERROR("Failed to read line: N/A.");
            return false;
        }

        //tell client we are good; got all the data
        if(!srv_send_msg("MRX", client)) {
            LOGERROR("Failed to receive acknowledgement from client that mechfile has been loaded.");
            return false;
        }
    }
    else {
        LOGERROR("Invalid client response: '%s'.", buf);
        return false;
    }

    close_file(&mechfile);

    return true;
}

uint get_line(const char* expected, file mechfile, int client) {

        char buf[2048];

        //request line
        if(!srv_send_msg("MRN", client)) {
            LOGERROR("Failed to request next mechfile line from player 1.");
            return false;
        }

        //receive line
        if(!srv_recv_msg(buf, client)) {
            LOGERROR("Failed to receive client response. Exiting program.");
            return false;
        }

        //check line
        if(!strcmp(expected, "N/A") == 0) {
            if(!(strcmp(buf, expected) == 0)){
                LOGERROR("Expected client to send line: %s. Exiting program.", expected);
                return false;
            }
        }

        //write line
        if(strcmp(buf, "\n") == 0) {
            if(!write_line(mechfile, "")) {
                LOGERROR("Failed to write to player 1 mechfile. Exiting program.");
                return false;
            }
        }
        else {
            if(!write_line(mechfile, buf)) {
                LOGERROR("Failed to write to player 1 mechfile. Exiting program.");
                return false;
            }
        }

        return true;
}

uint send_mechfile(int client) {

    file mechfile;

    if(client == P1) {
        if(!open_file("../server/mechfiles/mech2.txt", &mechfile, "r")) {
            LOGERROR("Failed to open requested mechfile for client %d.", client);
            return false;
        }
    }
    else {
        if(!open_file("../server/mechfiles/mech1.txt", &mechfile, "r")) {
            LOGERROR("Failed to open requested mechfile for client %d.", client);
            return false;
        }
    }

    //send "mts"
    if(!srv_send_msg("MTS", client)) {
        LOGERROR("Failed to send MTS message to client %d.", client);
        return false;
    }

    uint result = 1;
    while(result == 1) {
        uint result = send_line(client, mechfile);
        if(result == 2) {
            if(!srv_send_msg("MTX", client)) {
                LOGERROR("Failed to send MTX message to client %d.", client);
                return false;
            }
            break;
        }
        if(result == 0) {
            return false;
        }
    }

    char buf[MAX_MESSAGE_SIZE];
    if(!srv_recv_msg(buf, client)) {
        return false;
    }
    if(!(strcmp(buf, "ACK") == 0)) {
        LOGERROR("Failed to receive ACK message from client %d.", client);
        return false;
    }

    LOGINFO("successfully sent mechfile to client %d.", client);

    close_file(&mechfile);

    return true;
}

uint send_line(int client, file mechfile) {

    char buf[MAX_MESSAGE_SIZE];
    char msg[MAX_MESSAGE_SIZE];

    //wait for ACK
    if(!srv_recv_msg(msg, client)) {
        LOGERROR("send_line: failed to receive message from client.");
        return false;
    }
    if(!(strcmp(msg, "ACK") == 0)) {
        LOGERROR("send_line: expected to receive an 'ACK' from client.");
        return false;
    }

    //read line
    //TODO: if end of file, stop
    const char* line = read_line(mechfile);
    if(line == NULL) {
        return 2;
    }
    strcpy(buf, line); //bug happening here
    int str_len = strlen(buf);
    if(str_len > 1) {
        buf[str_len-1] = '\0'; //<- get rid of \n??
    }

    LOGINFO("buf: %s", buf);

    //send line
    if(!srv_send_msg(buf, client)) {
        LOGERROR("send_line: failed to send line to client.");
        return false;
    }

    return true;
}

uint process_command(char* command) {

    //TODO: have each command send update to client as to what happened

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
