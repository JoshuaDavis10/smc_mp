#pragma once

//for file I/O stuff
#include <stdio.h>

typedef struct file {
	FILE* handle;
	const char* mode;
} file;

int open_file(const char* path, file* file, const char* mode);

int close_file(file* file);

const char* read_line(file file);

int write_line(file file, const char* line);
