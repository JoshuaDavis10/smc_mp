#include "logger.h"

#if defined(PLATFORM_WEB)
#else
    #include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>

void log_message(message_type msgType, const char* msg, color color, ...) {

    #if defined(PLATFORM_WEB)
        char output[16384];

        va_list arg_ptr;

        va_start(arg_ptr, msg);
        vsnprintf(output, 16384, msg, arg_ptr);
        va_end(arg_ptr);

    	switch(msgType) {
    		case LINFO: 
    			printf("INFO: %s\n", output);
    			break;
    		case LTEXT: 
    			printf("%s\n", output);
    			break;
    		case LWARN: 
    			printf("WARN: %s\n", output);
    			break;
    		case LERROR:
    			printf("ERROR: %s\n", output);
    			break;
	}
    #else
	    HANDLE hConsole;
	    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        char output[16384];

        va_list arg_ptr;

        va_start(arg_ptr, color);
        vsnprintf(output, 16384, msg, arg_ptr);
        va_end(arg_ptr);

	    switch(msgType) {
	    	case LINFO: 
	    		SetConsoleTextAttribute(hConsole, color);
	    		printf("INFO: %s\n", output);
	    		SetConsoleTextAttribute(hConsole, LOGWHITE);
	    		break;
    		case LTEXT: 
	    		SetConsoleTextAttribute(hConsole, color);
    			printf("%s\n", output);
	    		SetConsoleTextAttribute(hConsole, LOGWHITE);
    			break;
	    	case LWARN: 
	    		SetConsoleTextAttribute(hConsole, color);
	    		printf("WARN: %s\n", output);
	    		SetConsoleTextAttribute(hConsole, LOGWHITE);
	    		break;
	    	case LERROR:
	    		SetConsoleTextAttribute(hConsole, color);
	    		printf("ERROR: %s\n", output);
	    		SetConsoleTextAttribute(hConsole, LOGWHITE);
	    		break;
	    }
    #endif
}
