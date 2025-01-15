#ifdef __linux__

#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_SIZE 1000
#define PORT "5001"

int* sockfd_ptr = 0;

//private functions
//determine if its ipv4 or ipv6?
void* get_in_addr(struct sockaddr *sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//private functions

//public functions
uint conn_to_serv(const char* hostname) {

    int sockfd;
    struct addrinfo hints, *servinfo, *ai_ptr;

    //getaddrinfo()
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(hostname, PORT, &hints, &servinfo) != 0) {
        printf("\e[1;31mERROR: getaddrinfo() did not return 0 as expected\e[0;37m\n");
        return false;
    }

    //loop to addrinfo that will work
    for(ai_ptr = servinfo; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {

        //socket()
        if((sockfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) == -1) {
            printf("\e[1;31mERROR: socket() returned -1\e[0;37m\n");
            continue;
        }

        //connect()
        if(connect(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) == -1) {
            close(sockfd);
            printf("\e[1;31mERROR: connect() returned -1\e[0;37m\n");
            continue;
        }

        break; //found good addrinfo and connected successfully
    }
    
    if(ai_ptr == NULL) {
            printf("\e[1;31mERROR: connection failed\e[0;37m\n");
            return false;
    }

    char s[INET6_ADDRSTRLEN];
    inet_ntop(ai_ptr->ai_family, get_in_addr((struct sockaddr*)ai_ptr->ai_addr), s, sizeof s); 
    //printf("DEBUG: connected to %s\n", s); //commented this out bc it prints IP address LOL
    printf("DEBUG: connected to server\n");

    freeaddrinfo(servinfo); //done with addressinfo

    sockfd_ptr = malloc(sizeof(int));
    memset(sockfd_ptr, 0, sizeof(int));
    *(sockfd_ptr) = sockfd;

    return true;
}

uint recv_msg(char* msg) {
    numbytes = recv(*(sockfd_ptr), msg, MAX_MESSAGE_SIZE-1, 0); 
    if(numbytes == -1) {
        printf("\e[1;31mERROR: recv() returned -1\e[0;37m\n");
        return false;
    }

    msg[numbytes] = '\0';
    return true;
}

uint send_msg(const char* msg) {
    if(send(*(sockfd_ptr), msg, strlen(msg), 0) == -1) {
        printf("\e[1;31mERROR: send() returned -1\e[0;37m\n");
        return false;
    }
    return true;
}
//public functions

#endif
