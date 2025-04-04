#pragma once
#include "shell_command_factory.h"

void ShellCommandFactory::setDriverInterface(SsdDriverInterface* pDriverInterface) {
    mpDriverInterface = pDriverInterface;
}

shared_ptr<ShellCommandInterface> ShellCommandFactory::getCommand(ShellCommand cmd) {
    switch (cmd) {
        case SSD_READ_COMMAND:
            return std::make_shared<ShellReadCommand>(mpDriverInterface);
        case SSD_WRITE_COMMAND:
            return std::make_shared<ShellWriteCommand>(mpDriverInterface);
        case SSD_FULLREAD_COMMAND:
            return std::make_shared<ShellFullReadCommand>(mpDriverInterface);
        case SSD_FULLWRITE_COMMAND:
            return std::make_shared<ShellFullWriteCommand>(mpDriverInterface);
        case SSD_HELP_COMMAND:
            return std::make_shared<ShellHelpCommand>();
        case SSD_EXIT_COMMAND:
            return std::make_shared<ShellExitCommand>();
        case SSD_ERASE_COMMAND:
            return std::make_shared<ShellEraseCommand>(mpDriverInterface);
        case SSD_ERASERANGE_COMMAND:
            return std::make_shared<ShellEraseRangeCommand>(mpDriverInterface);
        case SSD_FLUSH_COMMAND:
            return std::make_shared<ShellFlushCommand>(mpDriverInterface);
    }
    return std::make_shared<ShellScriptRunnerCommand>();
}