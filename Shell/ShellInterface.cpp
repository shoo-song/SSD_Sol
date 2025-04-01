#pragma once
#include <string>

enum ShellCommand {
	READ_COMMAND,
	WRITE_COMMAND,
	HELP_COMMAND,
	EXIT_COMMAND,
	FULLWRITE_COMMAND,
	FULLREAD_COMMAND,
	UNKOWN
};

using std::string;

class ShellInterface {
public:
	ShellCommand parse(string commandArg) {
		if (commandArg.compare("read") == 0) {
			return READ_COMMAND;
		}
		if (commandArg.compare("write") == 0) {
			return WRITE_COMMAND;
		}
		if (commandArg.compare("help") == 0) {
			return HELP_COMMAND;
		}
		if (commandArg.compare("exit") == 0) {
			return EXIT_COMMAND;
		}
		if (commandArg.compare("fullwrite") == 0) {
			return FULLWRITE_COMMAND;
		}
		if (commandArg.compare("fullread") == 0) {
			return FULLREAD_COMMAND;
		}
		return UNKOWN;
	}
private:
};