#include "shell_full_write_command.h"

ShellFullWriteCommand::ShellFullWriteCommand(SsdDriverInterface* pDriverInterface) {
    mpDriverInterface = pDriverInterface;
}

string ShellFullWriteCommand::execute(vector<string> args) {
    try {
        vector<unsigned int> convertedArgs = convertCmdArgs(args);
        string fullWriteResult = "[Full Write] Done";

        for (int i = 0; i < 100; i++) {
            mpDriverInterface->writeSSD(i, convertedArgs[0]);
        }

        return fullWriteResult;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}

vector<unsigned int> ShellFullWriteCommand::convertCmdArgs(vector<string> args) {
    try {
        vector<unsigned int> output;
        if (args.size() != 2) {
            throw ShellArgConvertException("args parameter size invalid");
        }

        output.push_back(ShellUtil::getUtilObj().convertHexStringForData(args[1]));

        return output;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}
