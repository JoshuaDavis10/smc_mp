#pragma once

#include "defines.h"

typedef struct {
    int client_fd[2];
} connection_info;

uint srv_initialize(); 
uint srv_send_msg(const char* msg, int client);
uint srv_recv_msg(char* msg, int client);
