#include "shell_flush_command.h"

ShellFlushCommand::ShellFlushCommand(SsdDriverInterface* pDriverInterface) {
	mpDriverInterface = pDriverInterface;
}

string ShellFlushCommand::execute(vector<string> args) {
	try {
		string output = "[Flush] Done";

		mpDriverInterface->flushSSD();

		return output;
	}
	catch (ShellArgConvertException e) {
		throw e;
	}
	catch (exception e) {
		throw ShellArgConvertException("invalid args");
	}
}