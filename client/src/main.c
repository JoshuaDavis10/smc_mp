#include "../include/raylib.h"

#include "logger.h"
#include "socket.h"
#include "files.h"

#include "game_core.h"

#include <stdio.h> //TODO: replace with logger stuff
#include <string.h>

//forward declare private functions
uint send_mechfile(const char* mechname);
uint get_mechfile(); 
uint get_line(const char* expected, file mechfile);
//forward declare private functions

int main(int argc, char** argv) {

    if(argc < 3 || argc > 3) {
        LOGERROR("usage: ./client hostname mechfile_name(w/o extension)");
        return -1;
    }

    //connect to server
    if(!conn_to_serv(argv[1])) {
        LOGERROR("Failed to connect to server. Exiting program.");
        return -1;
    }
    //connect to server

    //send mechfile
    if(!send_mechfile(argv[2])) {
        LOGERROR("Failed to send mechfile to server. Exiting program.");
        return -1;
    }
    //send mechfile

    //receive enemy mech file
    if(!get_mechfile()) {
        LOGERROR("Failed to get opponent's mechfile from server. Exiting program.");
        return -1;
    }
    //receive enemy mech file

    //initialize client-side version of game state
    //gc_initialize(const char* filepath1, const char* filepath2, int pos1, int pos2);
    //TODO: get random start positions from server
    //initialize client-side version of game state

    //create window
    InitWindow(1000, 500, "super mechs clone demo");
    //create window





    //"game" loop

    while(!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(GREEN);

        EndDrawing();

/*
        //receive message from server
        if(!recv_msg(buf)) {
            LOGERROR("Failed to receive message from server. Exiting program.");
            CloseWindow();
            return -1;
        }
        //receive message from server

        //if server is telling us to send a command (i.e. message from server is "A")...
        if(strcmp("A", buf) == 0) {
            // prompt for message sent
            printf("Your turn (enter command):\n");

            // send message
            fgets(msg, MAX_MESSAGE_SIZE, stdin);

            if(!send_msg(msg)) {
                LOGERROR("Failed to send message to server. Exiting program.");
                CloseWindow();
                return -1;
            }
        }
        //if server is telling us to send a command (i.e. message from server is "A")...

        //TODO: need else if case for if server notifies us that the game is over

        //otherwise, if server message is not "A"
        else {
            //TODO: rather than print info from server, we must process it and update our game state wrt it
            printf("%s\n", buf);
            send_msg(" ");
            //TODO: ofc we have to render the change in game state b4 actually changing it but yeah
            //TODO: rather than print info from server, we must process it and update our game state wrt it
        }
        //otherwise, if server message is not "A"
    */
    }
    //"game" loop

    CloseWindow();

    return 0;

}

uint send_mechfile(const char* mechname) {

    char buf[MAX_MESSAGE_SIZE];
    file mechfile;

    if(!recv_msg(buf)) {
        LOGERROR("Did not receive server message. Exiting program.");
        return false;
    }
    
    //if message is request for mech data
    if(strcmp("MRS", buf) == 0) {

        char mechfile_path[1024];
        //TODO: open mechfile
        snprintf(mechfile_path, 1024, "../client/mechfiles/%s.txt", mechname);
        LOGINFO("transmitting mech info from file: %s", mechfile_path);

        if(!open_file(mechfile_path, &mechfile, "r")) {
            LOGERROR("Failed to open mechfile: %s. Exiting program.", mechfile_path);
            return false;
        }

        //acknowledge request
        if(!send_msg("ACK")) {
            LOGERROR("Failed to 'ACK' to server notification. Exiting program.");
            return false;
        }

        //check to make sure server still wants data
        if(!recv_msg(buf)) {
            LOGERROR("Did not receive server message. Exiting program.");
            return false;
        }

        char buffer[2048];
        // - read one line from mechfile
        const char* line = read_line(mechfile);
        strcpy(buffer, line);           
        int str_len = strlen(buffer);
        if(str_len > 1) {
            buffer[str_len-1] = '\0'; //<- get rid of \n??
        }
        // - send that line to server
        if(!send_msg(buffer)) {
            LOGERROR("Failed to send message to server. Exiting program.");
            return false;
        }

        while(strcmp("MRN", buf) == 0) {
            if(!recv_msg(buf)) {
                LOGERROR("Did not receive server message. Exiting program.");
                return false;
            }
            if(strcmp("MRX", buf) == 0) {
                LOGINFO("Finished transmitting mech data to server.");
                if(!send_msg("ACK")) {
                    LOGERROR("Failed to 'ACK' to server notification. Exiting program.");
                    return false;
                }
                break;
            }

            // - read one line from mechfile
            line = read_line(mechfile);
            strcpy(buffer, line);           
            str_len = strlen(buffer);
            if(str_len > 1) {
                buffer[str_len-1] = '\0'; //<- get rid of \n??
            }
            // - send that line to server
            if(!send_msg(buffer)) {
                LOGERROR("Failed to send message to server. Exiting program.");
                return false;
            }
        }
    }
    //otherwise crash
    else {
        LOGERROR("Did not receive server request for mech data. Exiting program.");
        return false;
    }

    close_file(&mechfile);

    return true;
}

uint get_mechfile() {

    char buf[MAX_MESSAGE_SIZE];

    //wait for server to start transmitting
    if(!recv_msg(buf)) {
        LOGERROR("get_mechfile: failed to receive message from server.");
        return false;
    }

    //make sure we got an 'MTS' (mechfile transmission start)
    if(!(strcmp(buf, "MTS") == 0)) {
        LOGERROR("get_mechfile: expected 'MTS' from server but got '%s'", buf);
        return false;
    }

    //open mechfile for opponent's mech
    file mechfile;
    if(!open_file("../client/mechfiles/opp.txt", &mechfile, "w")) {
        LOGERROR("get_mechfile: failed to open file for opponent mech.");
        return false;
    }

    if(!get_line("#name", mechfile)) {
        LOGERROR("Failed to read line: #name.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("\n", mechfile)) {
        LOGERROR("Failed to read line: \\n.");
        return false;
    }
    if(!get_line("#health", mechfile)) {
        LOGERROR("Failed to read line: #health.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("\n", mechfile)) {
        LOGERROR("Failed to read line: \\n.");
        return false;
    }
    if(!get_line("#movement", mechfile)) {
        LOGERROR("Failed to read line: #health.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("\n", mechfile)) {
        LOGERROR("Failed to read line: \\n.");
        return false;
    }
    if(!get_line("#weapon", mechfile)) {
        LOGERROR("Failed to read line: #health.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("\n", mechfile)) {
        LOGERROR("Failed to read line: \\n.");
        return false;
    }
    if(!get_line("#weapon", mechfile)) {
        LOGERROR("Failed to read line: #health.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }
    if(!get_line("N/A", mechfile)) {
        LOGERROR("Failed to read line: N/A.");
        return false;
    }

    if(!send_msg("ACK")) {
        LOGERROR("Failed to send ACK to server.");
        return false;
    }

    if(!recv_msg(buf)) {
        LOGERROR("Failed to receive MTX message from server.");
        return false;
    }

    if(!(strcmp(buf, "MTX") == 0)) {
        LOGERROR("Failed to receive MTX message from server.");
        return false;
    }

    if(!send_msg("ACK")) {
        LOGERROR("Failed to send ACK message to server.");
        return false;
    }

    close_file(&mechfile);

    LOGINFO("successfully got opponent's mechfile from server.");
    return true;
}

uint get_line(const char* expected, file mechfile) {

    char buf[MAX_MESSAGE_SIZE];

    //request line
    if(!send_msg("ACK")) {
        LOGERROR("get_line: failed to send 'ACK' to server.");
        return false;
    }

    //receive line
    if(!recv_msg(buf)) {
        LOGERROR("get_line: failed to get next mechfile line from server.");
        return false;
    }

    //check line
    if(!strcmp(expected, "N/A") == 0) {
        if(!(strcmp(buf, expected) == 0)){
            LOGERROR("get_line: expected server to send line: %s.", expected);
            return false;
        }
    }

    //write line
    if(strcmp(buf, "\n") == 0) {
        if(!write_line(mechfile, "")) {
            LOGERROR("get_line: failed to write to opponent mechfile.");
            return false;
        }
    }
    else {
        if(!write_line(mechfile, buf)) {
            LOGERROR("get_line: failed to write to opponent mechfile.");
            return false;
        }
    }

    return true;
}
