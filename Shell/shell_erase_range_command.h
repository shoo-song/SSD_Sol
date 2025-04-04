#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellEraseRangeCommand : public ShellCommandInterface {
public:
	ShellEraseRangeCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "[Full Erase] Done";

			mpDriverInterface->eraseSSD((int)convertedArgs[0], (int)convertedArgs[1]);

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
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;

			if (args.size() != 3) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			// LBA String으로 변환.
			output.push_back(ShellUtil::getUtilObj().convertDecimalStringForLba(args[1]));
			output.push_back(ShellUtil::getUtilObj().convertLBAtoSize(args[1], args[2]));

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