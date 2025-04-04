#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellFlushCommand : public ShellCommandInterface {
public:
	ShellFlushCommand(SsdDriverInterface* pDriverInterface);
	string execute(vector<string> args);
private:
	SsdDriverInterface* mpDriverInterface;
};