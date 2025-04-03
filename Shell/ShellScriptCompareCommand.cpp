#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"

class ShellScriptCompareCommand : public ShellScriptCommandInterface {
public:
	ShellScriptCompareCommand(SsdDriverInterface* pDriverInterface, vector<string> args) {
		try {
			mpDriverInterface = pDriverInterface;
			mConvertedVec = convertCmdArgs(args);
		}
		catch (exception e) {
			throw e;
		}
	}

	bool execute() {
		try {
			unsigned int readValue = mpDriverInterface->readSSD((int)mConvertedVec[0]);
			return (readValue == mConvertedVec[1]);
		}
		catch (exception e) {
			return false;
		}
	}
private:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;

			if (args.size() != 3) {
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
	vector<unsigned int> mConvertedVec;
};