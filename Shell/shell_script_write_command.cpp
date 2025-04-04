#include "shell_script_write_command.h"

ShellScriptWriteCommand::ShellScriptWriteCommand(SsdDriverInterface* pDriverInterface,
	shared_ptr<ShellScriptParameterGenInterface> lbaParam,
	shared_ptr<ShellScriptParameterGenInterface> writeValueParam) {
	mLbaParam = lbaParam;
	mWriteValueParam = writeValueParam;
	mpDriverInterface = pDriverInterface;
}

bool ShellScriptWriteCommand::execute() {
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