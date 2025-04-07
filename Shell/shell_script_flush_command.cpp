#include "shell_script_flush_command.h"
#include "ssddriver_store.h"
#include <exception>

using std::exception;

ShellScriptFlushCommand::ShellScriptFlushCommand() {
}

bool ShellScriptFlushCommand::execute() {
    try {
        SsdDriverStore::getSsdDriverStore().getSsdDriver()->flushSSD();
        return true;
    }
    catch (exception e) {
        return false;
    }
}