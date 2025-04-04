
// #define UNIT_TEST

#ifdef UNIT_TEST
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#if 0
#include "logger.h"
#include "shell.h"

int main(int argc, char** argv) {
    logger.setLogFile();
    logger.print("Main.main()", "∑Œ±Î Ω√¿€");

    Shell shell;
    shell.runShell(argc, argv);
    return 0;
}
#endif
#endif
