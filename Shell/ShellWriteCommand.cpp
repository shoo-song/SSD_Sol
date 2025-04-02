#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellInterfaceUtil.cpp"

class ShellWriteCommand : public ShellCommandInterface {
public:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;
			if (args.size() != 2) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			output.push_back(ShellInterfaceUtil::getUtilObj().convertDecimalStringForLba(args[0]));
			output.push_back(ShellInterfaceUtil::getUtilObj().convertHexStringForData(args[1]));

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
};