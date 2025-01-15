#ifdef _WIN32

#include "socket.h"

#include "logger.h"

#include <winsock2.h>
#include <Windows.h>

#include <stdlib.h>
#include <string.h>

#define PORT 5001
#define BACKLOG 10

typedef struct {
    SOCKET client_fd[2];
} connection_info;

static connection_info* ci = 0;

//private function declarations
SOCKET setup_socket();
//private function declarations

//public functions
uint srv_initialize() {

    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    SOCKET sock = setup_socket();

    LOGINFO("SERVER: socket created");
    LOGINFO("SERVER: waiting for connections...");

    //TODO: error check accept() calls
    SOCKET cl_1 = accept(sock, 0, 0); 
    LOGINFO("SERVER: received connection from first client!");
    SOCKET cl_2 = accept(sock, 0, 0);
    LOGINFO("SERVER: received connection from second client!");

    ci = malloc(sizeof(connection_info));
    memset(ci, 0, sizeof(connection_info));

    ci->client_fd[P1] = cl_1;
    ci->client_fd[P2] = cl_2;

    return true;
}

uint srv_send_msg(const char* msg, int client) {
    LOGINFO("SERVER: sending message: %s.", msg);

    if(send(ci->client_fd[client], msg, strlen(msg), 0) == -1) {
        LOGWARN("SERVER: message to client %d failed to send. send() returned -1", client);
        return false;
    }

    return true;
}

uint srv_recv_msg(char* msg, int client) {
    if(client != P1 && client != P2) {
        LOGWARN("SERVER: invalid param value for 'client': %d (must be %d (P1) or %d (P2))", client, P1, P2);
        return false;
    }

    char client_msg[MAX_MESSAGE_SIZE];
    int numbytes = recv(ci->client_fd[client], client_msg, MAX_MESSAGE_SIZE-1, 0);
    if(numbytes == -1) {
        LOGWARN("SERVER: failed to receive message from client %d. recv() returned -1", client);
        return false;
    }
    client_msg[numbytes-1] = '\0';

    LOGINFO("SERVER: received message from client %d: %s", client, client_msg);

    strcpy(msg, client_msg);

    return true;
}
//public functions

//private functions
SOCKET setup_socket() {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0); //TODO: error check socket() call
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(PORT);
    bind(s, (SOCKADDR*) &addr, sizeof(addr)); //TODO: error check bind() call

    listen(s, BACKLOG); //TODO: error check listen() call

    return s;
}
//private functions

#endif
