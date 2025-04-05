#include "shell_executor.h"

string ShellExecutor::execute(string input, bool isRunnerMode) {
    try {
        vector<string> separatedStr = ShellUtil::getUtilObj().splitString(input);

        ShellCommand cmd = ShellUtil::getUtilObj().parse(separatedStr[0]);
        if (isRunnerMode && (cmd != SSD_SCRIPT_RUN_COMMAND)) {
            throw ShellException("invalid command");
        }
        shared_ptr<ShellCommandInterface> commandExecutor = mCommandFactory.getCommand(cmd);
        if (commandExecutor == nullptr) {
            throw ShellException("invalid command");
        }

        return commandExecutor->execute(separatedStr);
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}
