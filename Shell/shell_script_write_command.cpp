#include "shell_script_write_command.h"
#include "ssddriver_store.h"

ShellScriptWriteCommand::ShellScriptWriteCommand(
    shared_ptr<ShellScriptParameterGenInterface> lbaParam,
    shared_ptr<ShellScriptParameterGenInterface> writeValueParam) {
    mLbaParam = lbaParam;
    mWriteValueParam = writeValueParam;
}

bool ShellScriptWriteCommand::execute() {
    try {
        unsigned int lba = mLbaParam->generateParameter();
        unsigned int value = mWriteValueParam->generateParameter();
        SsdDriverStore::getSsdDriverStore().getSsdDriver()->writeSSD((int)lba, (int)value);
        return true;
    } catch (exception e) {
        return false;
    }
}