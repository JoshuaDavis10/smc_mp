#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_BACKLOG 5 
#define MAX_MESSAGE_SIZE 1000
#define PORT "5001"

//determine if its ipv4 or ipv6?
void* get_in_addr(struct sockaddr *sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int setup_socket() {

    //declarations for socket
    struct addrinfo hints;      //will be used as 3rd param in getaddrinfo() (give getaddrinfo 'hints' about how u want to use it?)
    struct addrinfo* servinfo;  //pass as 4th param of getaddrinfo to get the linked list of addrinfo structs

//socket
    memset(&hints, 0, sizeof(struct addrinfo));// zero out memory
    hints.ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream
    hints.ai_flags = AI_PASSIVE;        // fill in IP for me (don't have to hardcode it)

    //getaddrinfo()
    //ig getaddrinfo should return 0 if its successful
    if((getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        printf("\e[1;31mERROR: getaddrinfo() did not return 0 as expected\e[0;37m\n");
        return -1;
    }

    //loop through addrinfo linked list until we can create and bind a socket with it
    struct addrinfo* ai_ptr;
    int sockfd; //socket ID?
    int yes = 1; //for setsockopt()... 4th param
    for(ai_ptr = servinfo; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
        //socket()
        if((sockfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) == -1) {
            printf("\e[1;33mWARNING: socket() returned -1\e[0;37m\n");
            continue; //go to next addrinfo struct in linked list
        }
        //setsockopt() <- allows to rerun server without having to wait a minute for the socket to clear after last run
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            printf("\e[1;31mERROR: setsockopt() returned -1\e[0;37m\n");
            return -1;
        }
        //bind() <- associate socket with an actual port
        if(bind(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) == -1) {
            close(sockfd); //close the socket we created since we can't bind it 
            printf("\e[1;33mWARNING: bind() returned -1\e[0;37m\n");
            continue; //go to next addrinfo struct in linked list
        }
        
        break; //end loop once we've successfully created/bound a socket 
    }

    freeaddrinfo(servinfo); //done with addrinfo struct

    //check that socket was actually bound
    if(ai_ptr == NULL) {
        printf("\e[1;31mERROR: failed to bind\e[0;37m\n");
        return -1;
    }

    //listen() <- wait for incoming connections with 
    if(listen(sockfd, MAX_BACKLOG) == -1) {
        printf("\e[1;31mERROR: listen() returned -1\e[0;37m\n");
        return -1;
    }
//socket

    return sockfd;

}

int main() {

    printf("DEBUG: *SERVER START*\n");

    int sockfd = setup_socket();

    printf("DEBUG: socket created.\n");
    printf("DEBUG: waiting for connections...\n");

    struct sockaddr_storage client_one_addr, client_two_addr; //used to store address info for client
    socklen_t client_socket_size = sizeof client_one_addr;
    int client_one_fd, client_two_fd; //ID for client socket

    //accept blocks until a connection comes in
    client_one_fd = accept(sockfd, (struct sockaddr*)&client_one_addr, &client_socket_size);
    if(client_one_fd == -1) {
        printf("\e[1;31mERROR: accept() returned -1\e[0;37m\n");
        return -1;
    }
    char s1[INET6_ADDRSTRLEN]; 
    inet_ntop(client_one_addr.ss_family, get_in_addr((struct sockaddr*)&client_one_addr), s1, sizeof s1);
    printf("DEBUG: got connection from %s\n", s1); //commenting this out bc it prints IP address LOL
    //printf("DEBUG: got connection from client 1\n");
    client_two_fd = accept(sockfd, (struct sockaddr*)&client_two_addr, &client_socket_size);
    if(client_two_fd == -1) {
        printf("\e[1;31mERROR: accept() returned -1\e[0;37m\n");
        return -1;
    }
    char s2[INET6_ADDRSTRLEN]; 
    inet_ntop(client_two_addr.ss_family, get_in_addr((struct sockaddr*)&client_two_addr), s2, sizeof s2);
    printf("DEBUG: got connection from %s\n", s2); //commenting this out bc it prints IP address LOL
    //printf("DEBUG: got connection from client 2\n");

    //loop and receive messages
    while(1) {
        //TODO: receive from client 1
        //TODO: send client 1's message to both clients
        //TODO: receive from client 2

    }

/* message stuff
        char msg[MAX_MESSAGE_SIZE];
        int numbytes = recv(clientfd, msg, MAX_MESSAGE_SIZE-1, 0);
        if(numbytes == -1) {
            printf("\e[1;31mERROR: recv() returned -1\e[0;37m\n");
            exit(1);
        }
        msg[numbytes] = '\0'; //null terminate the message so its a string lol
        printf("DEBUG: received message from client: %s\n", msg);

        char msg_string[MAX_MESSAGE_SIZE];
        snprintf(msg_string, MAX_MESSAGE_SIZE, "%s\n%d", server_name, server_number);
        printf("DEBUG: sending message: %s\n", msg_string);
        if(send(clientfd, msg_string, strlen(msg_string), 0) == -1) {
            printf("\e[1;31mERROR: send() returned -1\e[0;37m\n");
            exit(3);
        }

        //close sockets
        close(sockfd);
        close(clientfd);
 message stuff */

        return 0;
}
