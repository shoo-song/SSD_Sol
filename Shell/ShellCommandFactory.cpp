#pragma once

#include "Common.h"
#include "ShellCommandInterface.h"
#include "ShellReadCommand.cpp"
#include "ShellWriteCommand.cpp"
#include <memory>

using std::shared_ptr;

class ShellCommandFactory {
public:
	shared_ptr<ShellCommandInterface> getCommand(ShellCommand cmd) {
		switch (cmd) {
			case READ_COMMAND:
				return std::make_shared<ShellReadCommand>();
			case WRITE_COMMAND:
				return std::make_shared<ShellWriteCommand>();
		}
		return nullptr;
	}

};