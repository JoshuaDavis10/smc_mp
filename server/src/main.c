#include "game_core.h"
#include "logger.h"

int main() {

    if(!gc_initialize("../server/test_mechs/mech1.txt", "../server/test_mechs/mech2.txt", 1, 2)) {
        LOGERROR("failed to initialize game core.");
        return -1;
    }

    if(!gc_shutdown()) {
        LOGERROR("game core was unable to shutdown correctly.");
        return -1;
    }

    return 0;
}
