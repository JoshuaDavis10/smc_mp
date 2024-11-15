#include "logger.h"

#include <stdio.h>
#include <stdarg.h>

void log_message(message_type msgType, const char* msg, ...) {

    char output[16384];
    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(output, 16384, msg, arg_ptr);
    va_end(arg_ptr);

	switch(msgType) {
		case LINFO: 
			printf("\e[1;32mINFO: %s\n\e[0;37m", output);
			break;
		case LWARN: 
			printf("\e[1;33mWARN: %s\n\e[0;37m", output);
			break;
		case LERROR:
			printf("\e[1;31mERROR: %s\n\e[0;37m", output);
			break;
    }
}
