#pragma once
#include "shell_util.h"
#include "shell_command_factory.h"
#include "ssddriver_interface.h"
#include <sstream>

class ShellExecutor {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface);
	string execute(string input, bool isRunnerMode);
private:
	SsdDriverInterface* mpDriverInterface;
	ShellCommandFactory mCommandFactory;
};