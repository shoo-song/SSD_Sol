#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellFullWriteCommand : public ShellCommandInterface {
public:
	ShellFullWriteCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string fullWriteResult = "[Full Write] Done";

			for (int i = 0; i < 100; i++) {

				mpDriverInterface->writeSSD(i, convertedArgs[0]);
			}

			return fullWriteResult;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}
private:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;
			if (args.size() != 2) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			output.push_back(ShellUtil::getUtilObj().convertHexStringForData(args[1]));

			return output;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

	SsdDriverInterface* mpDriverInterface;
};