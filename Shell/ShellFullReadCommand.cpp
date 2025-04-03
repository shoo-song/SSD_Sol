#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"

class ShellFullReadCommand : public ShellCommandInterface {
public:
	ShellFullReadCommand(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
	}

	string execute(vector<string> args) {
		try {
			string fullreadResult = "";

			for (int i = 0; i < 100; i++) {
				string hexString = ShellUtil::getUtilObj().toHexFormat(mpDriverInterface->readSSD(i));

				fullreadResult += "[Full Read] LBA ";
				fullreadResult += ShellUtil::getUtilObj().toTwoDigitString(i) + " ";
				fullreadResult += hexString + " \n";
			}

			return fullreadResult;
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