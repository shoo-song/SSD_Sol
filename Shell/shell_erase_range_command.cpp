#include "shell_erase_range_command.h"
#include "ssddriver_store.h"

ShellEraseRangeCommand::ShellEraseRangeCommand() {
}

string ShellEraseRangeCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string output = "";

        int startLBA = (int)convertedArgs[0];
        int rangeLBA = (int)convertedArgs[1];
        int maxChunkSize = 10;

        while (rangeLBA > 0) {
            int chunk = std::min(rangeLBA, maxChunkSize);
            SsdDriverStore::getSsdDriverStore().getSsdDriver()->eraseSSD(startLBA, chunk);
            output += "[Erase] Done " + std::to_string(startLBA) + ' ' + std::to_string(chunk) + '\n';
            startLBA += chunk;
            rangeLBA -= chunk;
        }

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
