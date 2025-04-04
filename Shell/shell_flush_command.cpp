#pragma once
#include "shell_command_interface.h"
#include "shell_exception.cpp"
#include "shell_util.cpp"
#include "ssddriver_interface.h"

class ShellFlushCommand : public ShellCommandInterface {
public:
	ShellFlushCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
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
private:
	SsdDriverInterface* mpDriverInterface;
};