#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellReadCommand: public ShellCommandInterface {
public:
	ShellReadCommand(SsdDriverInterface* pDriverInterface);
	string execute(vector<string> args);
private:
	vector<unsigned int>  convertCmdArgs(vector<string> args);
	SsdDriverInterface* mpDriverInterface;
};