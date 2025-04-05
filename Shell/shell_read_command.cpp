#include "shell_read_command.h"
#include "ssddriver_store.h"

ShellReadCommand::ShellReadCommand() {
}

string ShellReadCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string output = "[Read] LBA ";

        unsigned int result = SsdDriverStore::getSsdDriverStore().getSsdDriver()->readSSD((int)convertedArgs[0]);

        output += ShellUtil::getUtilObj().toTwoDigitString(convertedArgs[0]);
        output += " : ";
        output += ShellUtil::getUtilObj().toHexFormat(result);

        return output;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}

vector<unsigned int> ShellReadCommand::convertCmdArgs(vector<string> args) {
    try {
        vector<unsigned int> output;

        if (args.size() != 2) {
            throw ShellArgConvertException("args parameter size invalid");
        }

        output.push_back(ShellUtil::getUtilObj().convertDecimalStringForLba(args[1]));
        return output;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}
