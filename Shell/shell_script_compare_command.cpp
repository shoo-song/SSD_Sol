
#include "shell_script_compare_command.h"

ShellScriptCompareCommand::ShellScriptCompareCommand(
    SsdDriverInterface* pDriverInterface, shared_ptr<ShellScriptParameterGenInterface> lbaParam,
    shared_ptr<ShellScriptParameterGenInterface> expectedValueParam) {
    mLbaParam = lbaParam;
    mExpectedValueParam = expectedValueParam;
    mpDriverInterface = pDriverInterface;
}

bool ShellScriptCompareCommand::execute() {
    try {
        unsigned int lba = mLbaParam->generateParameter();
        unsigned int expectedValue = mExpectedValueParam->generateParameter();
        unsigned int readValue = mpDriverInterface->readSSD(lba);
        return (readValue == expectedValue);
    } catch (exception e) {
        return false;
    }
}