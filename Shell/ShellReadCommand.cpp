#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"

class ShellReadCommand: public ShellCommandInterface {
public:
	ShellReadCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			vector<unsigned int> convertedArgs = convertCmdArgs(args);
			string output = "[Read] LBA ";
			
			unsigned int result = mpDriverInterface->readSSD((int)convertedArgs[0]);

			output += ShellUtil::getUtilObj().toTwoDigitString(convertedArgs[0]);
			output += " : ";
			output += ShellUtil::getUtilObj().toHexFormat(result);

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

			if (args.size() != 2) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			output.push_back(ShellUtil::getUtilObj().convertDecimalStringForLba(args[1]));
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