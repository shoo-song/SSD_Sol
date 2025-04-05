#include "shell_script_erase_command.h"

ShellScriptEraseCommand::ShellScriptEraseCommand(
    SsdDriverInterface* pDriverInterface,
    shared_ptr<ShellScriptParameterGenInterface> startLbaParam,
    shared_ptr<ShellScriptParameterGenInterface> endLbaParam) {
    mStartLbaParam = startLbaParam;
    mEndValueParam = endLbaParam;
    mpDriverInterface = pDriverInterface;
}

bool ShellScriptEraseCommand::execute() {
    try {
        unsigned int startLba = mStartLbaParam->generateParameter();
        unsigned int endLba = mEndValueParam->generateParameter();
        mpDriverInterface->eraseSSD((int)startLba, (int)((endLba - startLba) + 1));
        return true;
    } catch (exception e) {
        return false;
    }
}