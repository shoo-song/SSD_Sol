#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
#include "ScriptParameterGenerator.h"
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