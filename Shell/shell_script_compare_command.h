#pragma once
#include "shell_script_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"
#include "script_parameter_generator.h"
#include <memory>

using std::shared_ptr;

class ShellScriptCompareCommand : public ShellScriptCommandInterface {
public:
	ShellScriptCompareCommand(SsdDriverInterface* pDriverInterface,
		shared_ptr<ShellScriptParameterGenInterface> lbaParam,
		shared_ptr<ShellScriptParameterGenInterface> expectedValueParam) {
		mLbaParam = lbaParam;
		mExpectedValueParam = expectedValueParam;
		mpDriverInterface = pDriverInterface;
	}

	bool execute() {
		try {
			unsigned int lba = mLbaParam->generateParameter();
			unsigned int expectedValue = mExpectedValueParam->generateParameter();
			unsigned int readValue = mpDriverInterface->readSSD(lba);
			return (readValue == expectedValue);
		}
		catch (exception e) {
			return false;
		}
	}
private:
	SsdDriverInterface* mpDriverInterface;
	shared_ptr<ShellScriptParameterGenInterface> mLbaParam;
	shared_ptr<ShellScriptParameterGenInterface> mExpectedValueParam;
};