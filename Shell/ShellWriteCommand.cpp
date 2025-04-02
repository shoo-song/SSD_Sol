#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellInterfaceUtil.cpp"
#include "ssddriver_interface.h"

class ShellWriteCommand : public ShellCommandInterface {
public:
	ShellWriteCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "[Write] Done";

			mpDriverInterface->writeSSD((int)convertedArgs[0], convertedArgs[1]);

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

			output.push_back(ShellInterfaceUtil::getUtilObj().convertDecimalStringForLba(args[1]));
			output.push_back(ShellInterfaceUtil::getUtilObj().convertHexStringForData(args[2]));

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