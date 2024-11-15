#include "../game_core.h"

#include "../logger.h"

int main() {
    if(!gc_initialize("~/dev/smc_mp/server/test_mechs/mech1.mch", "~/dev/smc_mp/server/test_mechs/mech2.mch", 1, 2)) {
        LOGERROR("failed to initialize game core.");
        return -1;
    }
    LOGINFO("initialized game core");
    return 0;
}
