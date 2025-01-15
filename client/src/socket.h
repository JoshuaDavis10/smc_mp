#pragma once

#include "defines.h"

uint conn_to_serv(const char* hostname); 
uint recv_msg(char* msg);
uint send_msg(const char* msg);
