#include "shell_flush_command.h"
#include "ssddriver_store.h"

ShellFlushCommand::ShellFlushCommand() {
}

string ShellFlushCommand::execute(vector<string> args) {
    try {
        string output = "[Flush] Done";

        SsdDriverStore::getSsdDriverStore().getSsdDriver()->flushSSD();

        return output;
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}