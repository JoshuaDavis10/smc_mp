#include "files.h"

#include <string.h>
#include <stdlib.h>

//function to open give file
int open_file(const char* path, file* file, const char* mode) {

	//open file @ filename
	FILE* opened_file;
	opened_file = fopen(path, mode);

	//set file handle
	file->handle = opened_file;
	file->mode = mode;

	//return 1 on success; 0 on failure.
	if(opened_file == 0) {
		return 0;
	}

	return 1;
}

//function to close given file
int close_file(file* file) {

	//close file
	int result = fclose(file->handle);

	//on success, return 1; on failure return 0.
	if(result == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

//read a line from the given file
const char* read_line(file file) {
	char buffer[32000];
	if(fgets(buffer, 32000, file.handle) != 0) {
		int length = strlen(buffer);
		char lineBuffer[length];

		strcpy(lineBuffer, buffer);
		const char* constLine = lineBuffer;
		return constLine;
	}
	return NULL;
}

//write 'line' to the given file 
//TODO: should not pass pointer cuz that could accidentally change file handle?
int write_line(file file, const char* line) {

	//TODO: comparison here is undefined so fix it so that you can't write to a file in write mode that doesn't
	//have that permission

	/*
	if(file->mode != "w" || file->mode != "r+") {
		printf("Error. Do not have permission to write to this file.\n");
		return 0;
	}
	*/

	//TODO: I feel like there's a 'better' way to do this
	char line_to_write[32000];
	strcpy(line_to_write, line);
	strcat(line_to_write, "\n");

	if(fputs(line_to_write, file.handle) == EOF) {
		//return 0 for error
		printf("Error. Failed to write line: '%s' to file.\n", line);
		return 0;
	}

	return 1;
}
