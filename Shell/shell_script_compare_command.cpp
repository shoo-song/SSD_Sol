
#include "shell_script_compare_command.h"
#include "ssddriver_store.h"
#include <exception>

using std::exception;

ShellScriptCompareCommand::ShellScriptCompareCommand(
    shared_ptr<ShellScriptParameterGenInterface> lbaParam,
    shared_ptr<ShellScriptParameterGenInterface> expectedValueParam) {
    mLbaParam = lbaParam;
    mExpectedValueParam = expectedValueParam;
}

bool ShellScriptCompareCommand::execute() {
    try {
        unsigned int lba = mLbaParam->generateParameter();
        unsigned int expectedValue = mExpectedValueParam->generateParameter();
        unsigned int readValue = SsdDriverStore::getSsdDriverStore().getSsdDriver()->readSSD(lba);
        return (readValue == expectedValue);
    } catch (exception e) {
        return false;
    }
}