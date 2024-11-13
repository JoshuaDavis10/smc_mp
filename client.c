#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_SIZE 1000
#define PORT "5001"

//determine if its ipv4 or ipv6?
void* get_in_addr(struct sockaddr *sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char** argv) {

    if(argc > 2 || argc < 2) {
        printf("\e[1;31mERROR: usage: server name\e[0;37m\n");
        return -1;
    }
    
    printf("DEBUG: *CLIENT START*\n");

//socket
    int sockfd;
    struct addrinfo hints, *servinfo, *ai_ptr;

    //getaddrinfo()
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(argv[1], PORT, &hints, &servinfo) != 0) {
        printf("\e[1;31mERROR: getaddrinfo() did not return 0 as expected\e[0;37m\n");
        return -1;
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
            return -1;
    }

    char s[INET6_ADDRSTRLEN];
    inet_ntop(ai_ptr->ai_family, get_in_addr((struct sockaddr*)ai_ptr->ai_addr), s, sizeof s); 
    //printf("DEBUG: connected to %s\n", s); //commented this out bc it prints IP address LOL
    printf("DEBUG: connected to server\n");

    freeaddrinfo(servinfo); //done with addressinfo

    while(1) {
        //TODO: receive message
        char buf[MAX_MESSAGE_SIZE];
        int numbytes = recv(sockfd, buf, MAX_MESSAGE_SIZE-1, 0); 
        if(numbytes == -1) {
            printf("\e[1;31mERROR: recv() returned -1\e[0;37m\n");
            return -1;
        }
        buf[numbytes] = '\0';

        //TODO: print received message
        printf("\e[1;32m- %s\n\e[0;37m", buf);


        //TODO: send message
        printf("- ");
        char msg[MAX_MESSAGE_SIZE];
        scanf("%[^\n]", msg);

        if(send(sockfd, msg, strlen(msg), 0) == -1) {
            printf("\e[1;31mERROR: send() returned -1\e[0;37m\n");
            return -1;
        }

    }

/*
    //send()
    char* msg_string = "Client of Joshua Davis";
    char send_msg[MAX_MESSAGE_SIZE];
    snprintf(send_msg, MAX_MESSAGE_SIZE, "%s\n%d", msg_string, client_number);
    printf("DEBUG: sending message: %s\n", send_msg);
    if(send(sockfd, send_msg, strlen(send_msg), 0) == -1) {
        printf("\e[1;31mERROR: send() returned -1\e[0;37m\n");
        return -1;
    }

    //recv()
    char recv_msg[MAX_MESSAGE_SIZE];
    int numbytes = recv(sockfd, recv_msg, MAX_MESSAGE_SIZE-1, 0);
    if(numbytes == -1) {
        printf("\e[1;31mERROR: recv() returned -1\e[0;37m\n");
        return -1;
    }
    recv_msg[numbytes] = '\0'; //null terminate the message so its a string lol
    printf("DEBUG: received message from server: %s\n", recv_msg);

*/

    close(sockfd);

    return 0;
}
