#pragma once
#include "shell_command_interface.h"
#include "shell_exception.cpp"

class ShellExitCommand : public ShellCommandInterface {
public:
	ShellExitCommand() {
	}

	string execute(vector<string> args) {
		if (args.size() != 1) {
			throw ShellArgConvertException("args parameter size invalid");
		}
		exit(0);
		return "NA";
	}
private:
};