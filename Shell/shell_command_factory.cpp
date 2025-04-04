#pragma once
#include "shell_command_factory.h"

void ShellCommandFactory::setDriverInterface(SsdDriverInterface* pDriverInterface) {
    mpDriverInterface = pDriverInterface;
}

shared_ptr<ShellCommandInterface> ShellCommandFactory::getCommand(ShellCommand cmd) {
    switch (cmd) {
        case READ_COMMAND:
            return std::make_shared<ShellReadCommand>(mpDriverInterface);
        case WRITE_COMMAND:
            return std::make_shared<ShellWriteCommand>(mpDriverInterface);
        case FULLREAD_COMMAND:
            return std::make_shared<ShellFullReadCommand>(mpDriverInterface);
        case FULLWRITE_COMMAND:
            return std::make_shared<ShellFullWriteCommand>(mpDriverInterface);
        case HELP_COMMAND:
            return std::make_shared<ShellHelpCommand>();
        case EXIT_COMMAND:
            return std::make_shared<ShellExitCommand>();
        case ERASE_COMMAND:
            return std::make_shared<ShellEraseCommand>(mpDriverInterface);
        case ERASERANGE_COMMAND:
            return std::make_shared<ShellEraseRangeCommand>(mpDriverInterface);
        case FLUSH_COMMAND:
            return std::make_shared<ShellFlushCommand>(mpDriverInterface);
    }
    return std::make_shared<ShellScriptRunnerCommand>();
}