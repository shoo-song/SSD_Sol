#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"

class ShellScriptWriteCommand : public ShellScriptCommandInterface {
public:
	ShellScriptWriteCommand(SsdDriverInterface* pDriverInterface, vector<string> args) {
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
			mpDriverInterface->writeSSD(mConvertedVec[0], mConvertedVec[1]);
			return true;
		}
		catch (exception e) {
			return false;
		}
	}
private:
	vector<unsigned int> convertCmdArgs(vector<string> args) {
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