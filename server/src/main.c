#include "logger.h"

#include "gc_test.h"

int main() {
    if(!initialize_gc_test()) {
        LOGERROR("game core test failed to initialize");
        return -1;
    }
    run_gc_test();
    shutdown_gc_test();

    return 0;
}
