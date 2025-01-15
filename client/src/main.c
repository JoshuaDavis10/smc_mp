#include "logger.h"
#include "socket.h"

int main(int argc, char** argv) {

    if(!conn_to_serv(argv[1])) {
        LOGERROR("Failed to connect to server. Exiting program.");
        return -1;
    }

    char buf[MAX_MESSAGE_SIZE];
    char msg[MAX_MESSAGE_SIZE];

    while(1) {

        if(!recv_msg(buf)) {
            LOGERROR("Failed to receive message from server. Exiting program.");
            return -1;
        }

        if(strcmp("A", buf) == 0) {
            // prompt for message sent
            printf("Your turn (enter command):\n");

            // send message
            fgets(msg, MAX_MESSAGE_SIZE, stdin);

            if(!send_msg(msg)) {
                LOGERROR("Failed to send message to server. Exiting program.");
                return -1;
            }
        }
        else {
            // print received message
            printf("%s\n", buf);
            send_msg(" ");
        }
    }

    return 0;

}
