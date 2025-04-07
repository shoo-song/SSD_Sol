
// #define UNIT_TEST

#ifdef UNIT_TEST
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "shell.h"

int main(int argc, char** argv) {
    Shell shell;
    return shell.runShell(argc, argv);
}
#endif
#endif
