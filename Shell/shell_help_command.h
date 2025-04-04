#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "script_store.h"

class ShellHelpCommand : public ShellCommandInterface {
public:
	ShellHelpCommand();
	string execute(vector<string> args);
private:
};