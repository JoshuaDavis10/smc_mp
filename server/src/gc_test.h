#pragma once

#include "defines.h"

#define MAX_TESTS 1024

typedef struct {
    uint (*test)();
} test;

typedef struct {
    test tests[MAX_TESTS];
    uint num_tests;
} gc_test; 

uint initialize_gc_test();
void shutdown_gc_test();
void run_gc_test();
