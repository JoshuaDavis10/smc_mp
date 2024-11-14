#pragma once //honestly for now ima have 3 functions cuz lets not make it too complicated
//one function that is a catch all and then 3 macros to call specific versions
//WARN, INFO, ERROR

typedef enum message_type {
	LINFO,  
    LTEXT,
	LWARN,
	LERROR
} message_type;

//TODO: change the names to be more accurate to the actual colors
typedef enum color {
    LOGBLACK = 0,
    LOGBLUE = 1,
    LOGGREEN = 2,
    LOGCYAN = 3,
    LOGRED = 4,
    LOGPINK = 5,
    LOGYELLOW = 6,
    LOGWHITE = 7,
    LOGGREY = 8,
    LOGBOLDBLUE = 9,
    LOGBOLDGREEN = 10,
    LOGBOLDCYAN = 11,
    LOGBOLDRED = 12,
    LOGBOLDPINK = 13,
    LOGBOLDYELLOW = 14,
    LOGBOLDWHITE = 15
} color; 

#define LOGGRAY LOGGREY

void log_message(message_type msgType, const char* msg, color color, ...); 

#define LOGINFO(msg, ...)  log_message(LINFO, msg, LOGGREEN, ##__VA_ARGS__)
#define LOGTEXT(msg, color, ...)  log_message(LTEXT, msg, color, ##__VA_ARGS__)
#define LOGWARN(msg, ...)  log_message(LWARN, msg, LOGYELLOW, ##__VA_ARGS__)
#define LOGERROR(msg, ...) log_message(LERROR, msg, LOGRED, ##__VA_ARGS__)
