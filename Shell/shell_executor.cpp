#include "shell_executor.h"

ShellCommand ShellExecutor::parse(string commandArg) {
    if (commandArg.compare("read") == 0) {
        return SSD_READ_COMMAND;
    }
    if (commandArg.compare("write") == 0) {
        return SSD_WRITE_COMMAND;
    }
    if (commandArg.compare("help") == 0) {
        return SSD_HELP_COMMAND;
    }
    if (commandArg.compare("exit") == 0) {
        return SSD_EXIT_COMMAND;
    }
    if (commandArg.compare("fullwrite") == 0) {
        return SSD_FULLWRITE_COMMAND;
    }
    if (commandArg.compare("fullread") == 0) {
        return SSD_FULLREAD_COMMAND;
    }
    if (commandArg.compare("erase") == 0) {
        return SSD_ERASE_COMMAND;
    }
    if (commandArg.compare("erase_range") == 0) {
        return SSD_ERASERANGE_COMMAND;
    }
    if (commandArg.compare("flush") == 0) {
        return SSD_FLUSH_COMMAND;
    }
    return SSD_SCRIPT_RUN_COMMAND;
}

string ShellExecutor::execute(string input, bool isRunnerMode) {
    try {
        vector<string> separatedStr = ShellUtil::getUtilObj().splitString(input);

        ShellCommand cmd = parse(separatedStr[0]);
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
