#pragma once

#include "Common.h"
#include "ShellCommandInterface.h"
#include "ShellReadCommand.cpp"
#include "ShellWriteCommand.cpp"
#include "ShellFullReadCommand.cpp"
#include "ShellFullWriteCommand.cpp"
#include "ShellHelpCommand.cpp"
#include "ShellExitCommand.cpp"
#include "ShellScriptRunCommand.cpp"
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
		}
		return std::make_shared<ShellScriptRunnerCommand>();
	}
private:
	SsdDriverInterface* mpDriverInterface;
};