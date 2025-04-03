#pragma once
#include "ShellUtil.cpp"
#include "ShellCommandFactory.cpp"
#include "ssddriver_interface.h"
#include <sstream>

class ShellExecutor {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
		mCommandFactory.setDriverInterface(pDriverInterface);
	}

	string execute(string input) {

		try {
			vector<string> separatedStr = ShellUtil::getUtilObj().splitString(input);

			ShellCommand cmd = ShellUtil::getUtilObj().parse(separatedStr[0]);
			if (cmd == UNKOWN) {
				cmd = ShellUtil::getUtilObj().parseScript(separatedStr[0]);
			}
			shared_ptr<ShellCommandInterface> commandExecutor = mCommandFactory.getCommand(cmd);
			if (commandExecutor == nullptr) {
				throw ShellArgConvertException("invalid command");
			}

			return commandExecutor->execute(separatedStr);
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
	ShellCommandFactory mCommandFactory;
};