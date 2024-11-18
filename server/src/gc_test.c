#include "gc_test.h"

#include "logger.h"
#include "game_core.h"

#include <stdlib.h>
#include <string.h>

#define ASSERT(x)     \
    if(!x) {          \
        gc_shutdown(); \
        return false; \
    }

//forward declared tests
uint tw1();
uint tw2();
uint tw3();
uint tw4();
uint tw5();
uint tw6();
uint tj1();
uint tj2();
uint tj3();
uint tj4();
uint tj5();
uint tj6();
uint tj7();
uint tj8();
uint tr1();
uint tr2();
uint tr3();
uint tr4();
uint tr5();
uint tr6();
uint tuw1();
uint tuw2();
uint tuw3();
//forward declared tests

//game core test pointer
static gc_test* gct = 0;
//game core test pointer

//helper function
uint add_test(uint (*t)(void)) {
    if(gct == 0) {
        LOGWARN("add_test: called before gc_test was initialized");
        return false;
    }
    if(gct->num_tests >= MAX_TESTS-1) {
        LOGWARN("add_test: cannot add test. maximum number of tests reached");
        return false;
    }

    test test;
    test.test = t;
    gct->tests[gct->num_tests] = test;
    gct->num_tests++;
    return true;
}
//helper function

//public functions
uint initialize_gc_test() {
    if(gct != 0) {
        LOGWARN("initialize_gc_test: tried to initialize when already initialized");
        return false;
    }
    gct = (gc_test*)malloc(sizeof(gc_test));
    memset(gct, 0, sizeof(gc_test));
    gct->num_tests = 0;

    //add tests
    /* movement tests
    add_test(tw1); 
    add_test(tw2); 
    add_test(tw3); 
    add_test(tw4); 
    add_test(tw5); 
    add_test(tw6); 
    add_test(tj1);
    add_test(tj2);
    add_test(tj3);
    add_test(tj4);
    add_test(tj5);
    add_test(tj6);
    add_test(tj7);
    add_test(tj8);
    add_test(tr1);
    add_test(tr2);
    add_test(tr3);
    add_test(tr4);
    add_test(tr5);
    add_test(tr6);
    movement tests */
    add_test(tuw1);
    add_test(tuw2);
    add_test(tuw3);
    //add tests
    
    return true;
}

void shutdown_gc_test() {
    free(gct);
    gct = 0;
}

void run_gc_test() {
    int num_passed = 0;
    int num_failed = 0;
    for(int i = 0; i < gct->num_tests; i++) {
        if(!(gct->tests[i].test)()) {
            LOGERROR("TEST: failed");
            num_failed++;
        }
        else {
            LOGINFO("TEST: passed");
            num_passed++;
        }
    }
    LOGINFO("gc_test_results:\npassed: %d\nfailed: %d", num_passed, num_failed);
    LOGINFO("passed: %d", num_passed);
    if(num_failed > 0) {
        LOGERROR("failed: %d", num_failed);
    }
    else {
        LOGINFO("failed: %d", num_failed);
    }
}
//public functions

//tests

    //walk tests
uint tw1() {
    LOGINFO("TEST: running walk test 1...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-1, 1);
    uint result = gc_walk(DIR_RIGHT);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tw2() {
    LOGINFO("TEST: running walk test 2...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 0, 1);
    uint result = gc_walk(DIR_LEFT);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tw3() {
    LOGINFO("TEST: running walk test 3...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 3);
    uint result = gc_walk(DIR_RIGHT);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tw4() {
    LOGINFO("TEST: running walk test 4...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 1);
    uint result = gc_walk(DIR_LEFT);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tw5() {
    LOGINFO("TEST: running walk test 5...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    uint result = gc_walk(DIR_RIGHT);
    ASSERT(result == true);
    gc_shutdown();
    return true;
}
uint tw6() {
    LOGINFO("TEST: running walk test 6...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    uint result = gc_walk(DIR_LEFT);
    ASSERT(result == true);
    gc_shutdown();
    return true;
}
    //walk tests

    //jump tests
uint tj1() {
    LOGINFO("TEST: running jump test 1...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-1, 0);
    uint result = gc_jump(DIR_RIGHT);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-2, 0);
    result = gc_jump(DIR_RIGHT);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tj2() {
    LOGINFO("TEST: running jump test 2...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 0, 1);
    uint result = gc_jump(DIR_LEFT);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2);
    result = gc_jump(DIR_LEFT);
    ASSERT(result == false);
    gc_shutdown();
    return true;

}
uint tj3() {
    LOGINFO("TEST: running jump test 3...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    uint result = gc_jump(DIR_RIGHT);
    ASSERT(result == false);
    gc_shutdown();
    return true;

}
uint tj4() {
    LOGINFO("TEST: running jump test 4...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 3, 1);
    uint result = gc_jump(DIR_LEFT);
    ASSERT(result == false);
    gc_shutdown();
    return true;

}
uint tj5() {
    LOGINFO("TEST: running jump test 5...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2);
    uint result = gc_jump(DIR_RIGHT);
    ASSERT(result == true);
    gc_shutdown();
    return true;

}
uint tj6() {
    LOGINFO("TEST: running jump test 6...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 1);
    uint result = gc_jump(DIR_LEFT);
    ASSERT(result == true);
    gc_shutdown();
    return true;

}
uint tj7() {
    LOGINFO("TEST: running jump test 7...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 5);
    uint result = gc_jump(DIR_RIGHT);
    ASSERT(result == true);
    gc_shutdown();
    return true;

}
uint tj8() {
    LOGINFO("TEST: running jump test 8...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 5, 1);
    uint result = gc_jump(DIR_LEFT);
    ASSERT(result == true);
    gc_shutdown();
    return true;

}
    //jump tests

    //roll tests
uint tr1() {
    LOGINFO("TEST: running roll test 1...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-1, 0);
    uint result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-2, 0);
    result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-2, 0);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-3, 0);
    result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-3, 0);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", NUM_POSITIONS-3, 0);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tr2() {
    LOGINFO("TEST: running roll test 2...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 0, 5);
    uint result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 5);
    result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 5);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 5);
    result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 5);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 5);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tr3() {
    LOGINFO("TEST: running roll test 3...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2);
    uint result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 4);
    result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 4);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 4);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tr4() {
    LOGINFO("TEST: running roll test 4...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 2, 1);
    uint result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 3, 1);
    result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 3, 1);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == false);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 4, 1);
    result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 4, 1);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 4, 1);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tr5() {
    LOGINFO("TEST: running roll test 5...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 8);
    uint result = gc_roll(DIR_RIGHT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 8);
    result = gc_roll(DIR_RIGHT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 8);
    result = gc_roll(DIR_RIGHT, 3);
    ASSERT(result == true);
    gc_shutdown();
    return true;
}
uint tr6() {
    LOGINFO("TEST: running roll test 6...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 8, 1);
    uint result = gc_roll(DIR_LEFT, 1);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 8, 1);
    result = gc_roll(DIR_LEFT, 2);
    ASSERT(result == true);
    gc_shutdown();
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 8, 1);
    result = gc_roll(DIR_LEFT, 3);
    ASSERT(result == true);
    gc_shutdown();
    return true;
}
    //roll tests

    //use weapon tests
uint tuw1() {
    LOGINFO("TEST: running use weapon test 1...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 3);
    uint result = gc_use_weapon(1);
    ASSERT(result == false);
    gc_walk(DIR_LEFT); //go to next guy's turn to test that
    result = gc_use_weapon(1);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
uint tuw2() {
    LOGINFO("TEST: running use weapon test 2...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2);
    uint result = gc_use_weapon(1);
    ASSERT(result == true);
    result = gc_use_weapon(1);
    ASSERT(result == true);
    gc_shutdown();
    return true;
}
uint tuw3() {
    LOGINFO("TEST: running use weapon test 3...");
    gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2);
    uint result = gc_use_weapon(0);
    ASSERT(result == false);
    gc_walk(DIR_LEFT); //go to next guy's turn to test that
    result = gc_use_weapon(0);
    ASSERT(result == false);
    gc_shutdown();
    return true;
}
    //use weapon tests
//tests

