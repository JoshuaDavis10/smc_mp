#pragma once 

typedef enum message_type {
	LINFO,  
	LWARN,
	LERROR
} message_type;

void log_message(message_type msgType, const char* msg, ...); 

#define LOGINFO(msg, ...)  log_message(LINFO, msg, ##__VA_ARGS__)
#define LOGWARN(msg, ...)  log_message(LWARN, msg, ##__VA_ARGS__)
#define LOGERROR(msg, ...) log_message(LERROR, msg, ##__VA_ARGS__)
