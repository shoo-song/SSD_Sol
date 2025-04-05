#include "shell_write_command.h"
#include "ssddriver_store.h"

ShellWriteCommand::ShellWriteCommand() {
}

string ShellWriteCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string output = "[Write] Done";

        SsdDriverStore::getSsdDriverStore().getSsdDriver()->writeSSD((int)convertedArgs[0], convertedArgs[1]);

        return output;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}

vector<unsigned int> ShellWriteCommand::convertCmdArgs(vector<string> args) {
    try {
        vector<unsigned int> output;
        if (args.size() != 3) {
            throw ShellArgConvertException("args parameter size invalid");
        }

        output.push_back(ShellUtil::getUtilObj().convertDecimalStringForLba(args[1]));
        output.push_back(ShellUtil::getUtilObj().convertHexStringForData(args[2]));

        return output;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}
