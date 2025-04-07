#pragma once
#include "shell_command_factory.h"

shared_ptr<ShellCommandInterface> ShellCommandFactory::getCommand(ShellCommand cmd) {
    switch (cmd) {
        case SSD_READ_COMMAND:
            return std::make_shared<ShellReadCommand>();
        case SSD_WRITE_COMMAND:
            return std::make_shared<ShellWriteCommand>();
        case SSD_FULLREAD_COMMAND:
            return std::make_shared<ShellFullReadCommand>();
        case SSD_FULLWRITE_COMMAND:
            return std::make_shared<ShellFullWriteCommand>();
        case SSD_HELP_COMMAND:
            return std::make_shared<ShellHelpCommand>();
        case SSD_EXIT_COMMAND:
            return std::make_shared<ShellExitCommand>();
        case SSD_ERASE_COMMAND:
            return std::make_shared<ShellEraseCommand>();
        case SSD_ERASERANGE_COMMAND:
            return std::make_shared<ShellEraseRangeCommand>();
        case SSD_FLUSH_COMMAND:
            return std::make_shared<ShellFlushCommand>();
    }
    return std::make_shared<ShellScriptRunnerCommand>();
}