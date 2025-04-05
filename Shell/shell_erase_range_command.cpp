#include "shell_erase_range_command.h"
#include "ssddriver_store.h"

ShellEraseRangeCommand::ShellEraseRangeCommand() {
}

string ShellEraseRangeCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string output = "[Full Erase] Done";

        SsdDriverStore::getSsdDriverStore().getSsdDriver()->eraseSSD((int)convertedArgs[0], (int)convertedArgs[1]);

        return output;
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}

vector<unsigned int> ShellEraseRangeCommand::convertCmdArgs(vector<string> args) {
    try {
        vector<unsigned int> output;

        if (args.size() != 3) {
            throw ShellException("args parameter size invalid");
        }

        // LBA String으로 변환.
        output.push_back(ShellUtil::getUtilObj().convertDecimalStringForLba(args[1]));
        output.push_back(ShellUtil::getUtilObj().convertLBAtoSize(args[1], args[2]));

        return output;
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}
