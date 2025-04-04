#pragma once

#include "common.h"
#include "shell_command_interface.h"
#include "shell_read_command.h"
#include "shell_write_command.h"
#include "shell_full_read_command.h"
#include "shell_full_write_command.h"
#include "shell_help_command.h"
#include "shell_exit_command.h"
#include "shell_erase_command.h"
#include "shell_erase_range_command.h"
#include "shell_flush_command.h"
#include "shell_script_run_command.h"
#include <memory>

using std::shared_ptr;

class ShellCommandFactory {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	shared_ptr<ShellCommandInterface> getCommand(ShellCommand cmd) {
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
private:
	SsdDriverInterface* mpDriverInterface;
};