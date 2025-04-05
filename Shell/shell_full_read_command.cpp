#include "shell_full_read_command.h"
#include "ssddriver_store.h"

ShellFullReadCommand::ShellFullReadCommand() {
}

string ShellFullReadCommand::execute(vector<string> args) {
    try {
        string fullreadResult = "";

        for (int i = 0; i < 100; i++) {
            string hexString = ShellUtil::getUtilObj().toHexFormat(SsdDriverStore::getSsdDriverStore().getSsdDriver()->readSSD(i));

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
