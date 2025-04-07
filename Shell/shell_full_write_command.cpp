#include "shell_full_write_command.h"
#include "ssddriver_store.h"

ShellFullWriteCommand::ShellFullWriteCommand() {
}

string ShellFullWriteCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string fullWriteResult = "[Full Write] Done";

        for (int i = 0; i < 100; i++) {
            SsdDriverStore::getSsdDriverStore().getSsdDriver()->writeSSD(i, convertedArgs[0]);
        }

        return fullWriteResult;
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}

vector<unsigned int> ShellFullWriteCommand::convertCmdArgs(vector<string> args) {
    try {
        vector<unsigned int> output;
        if (args.size() != 2) {
            throw ShellException("args parameter size invalid");
        }

        output.push_back(ShellUtil::getUtilObj().convertHexStringForData(args[1]));

        return output;
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}
