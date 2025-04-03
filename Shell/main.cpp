
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
#include "ScriptParser.cpp"

int main(int argc, char** argv) {

    logger.setLogFile();
    logger.print("Main.main()", "∑Œ±Î Ω√¿€");

    //Shell shell;
    //shell.runShell();
    ScriptParser parser;
    ::vector<shared_ptr<ShellScriptCommandInterface>> scriptCmds = parser.MakingScript("1_FullWriteAndReadCompare");
    for (shared_ptr<ShellScriptCommandInterface> cmd : scriptCmds) {
        cmd->execute();
    }
    return 0;
}
#endif

