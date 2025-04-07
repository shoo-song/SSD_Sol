#include "shell_script_erase_command.h"
#include "ssddriver_store.h"
#include <exception>

using std::exception;

ShellScriptEraseCommand::ShellScriptEraseCommand(
    shared_ptr<ShellScriptParameterGenInterface> startLbaParam,
    shared_ptr<ShellScriptParameterGenInterface> endLbaParam) {
    mStartLbaParam = startLbaParam;
    mEndValueParam = endLbaParam;
}

bool ShellScriptEraseCommand::execute() {
    try {
        unsigned int startLba = mStartLbaParam->generateParameter();
        unsigned int endLba = mEndValueParam->generateParameter();
        SsdDriverStore::getSsdDriverStore().getSsdDriver()->eraseSSD((int)startLba, (int)((endLba - startLba) + 1));
        return true;
    } catch (exception e) {
        return false;
    }
}