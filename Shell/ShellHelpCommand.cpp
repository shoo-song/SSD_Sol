#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"

class ShellHelpCommand : public ShellCommandInterface {
public:
	ShellHelpCommand() {
	}

	string execute(vector<string> args) {
		if (args.size() != 1) {
			throw ShellArgConvertException("args parameter size invalid");
		}
		return "Team:Alpha Devs(ÀÌ¿øÃ¶/¼Û½ÂÈ£/½Åµ¿Àç/ÀüºÀ¼ö)\n"
			"READ command: read [LBA]\n"
			"Write command: write [LBA] [DATA:(ex)0x123456]\n"
			"Full Read command: fullread\n"
			"Full Write command: fullwrite\n"
			"Exit command: exit\n";
	}
private:
};