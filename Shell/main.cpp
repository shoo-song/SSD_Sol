
// #define UNIT_TEST

#ifdef UNIT_TEST
#include <gmock/gmock.h>

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
#include "Shell.cpp"
#include "logger.h"

int main(int argc, char** argv) {

    logger.setLogFile();
    logger.print("Main.main()", "∑Œ±Î Ω√¿€");

    int test_argc = 2;
    char* test_argv[] = { (char*)"shell.exe", (char*)"runner_script1.txt" };
    Shell shell;
    shell.runShell(test_argc, test_argv);
    return 0;
}
#endif

