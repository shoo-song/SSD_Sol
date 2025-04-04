#include "shell_full_read_command.h"

ShellFullReadCommand::ShellFullReadCommand(SsdDriverInterface* pDriverInterface) {
    mpDriverInterface = pDriverInterface;
}

string ShellFullReadCommand::execute(vector<string> args) {
    try {
        string fullreadResult = "";

        for (int i = 0; i < 100; i++) {
            string hexString = ShellUtil::getUtilObj().toHexFormat(mpDriverInterface->readSSD(i));

            fullreadResult += "[Full Read] LBA ";
            fullreadResult += ShellUtil::getUtilObj().toTwoDigitString(i) + " ";
            fullreadResult += hexString + " \n";
        }

        return fullreadResult;
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}
