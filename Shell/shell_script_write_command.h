#pragma once
#include "shell_script_command_interface.h"
#include "shell_util.h"
#include "ssddriver_interface.h"
#include "script_parameter_generator.h"
#include <memory>

using std::shared_ptr;

class ShellScriptWriteCommand : public ShellScriptCommandInterface {
public:
	ShellScriptWriteCommand(SsdDriverInterface* pDriverInterface,
		shared_ptr<ShellScriptParameterGenInterface> lbaParam,
		shared_ptr<ShellScriptParameterGenInterface> writeValueParam) {
		mLbaParam = lbaParam;
		mWriteValueParam = writeValueParam;
		mpDriverInterface = pDriverInterface;
	}

	bool execute() {
		try {
			unsigned int lba = mLbaParam->generateParameter();
			unsigned int value = mWriteValueParam->generateParameter();
			mpDriverInterface->writeSSD((int)lba, (int)value);
			return true;
		}
		catch (exception e) {
			return false;
		}
	}
private:
	SsdDriverInterface* mpDriverInterface;
	shared_ptr<ShellScriptParameterGenInterface> mLbaParam;
	shared_ptr<ShellScriptParameterGenInterface> mWriteValueParam;
};