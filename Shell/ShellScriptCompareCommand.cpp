#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
#include "ScriptParameterGenerator.h"
#include <memory>

using std::shared_ptr;

class ShellScriptCompareCommand : public ShellScriptCommandInterface {
public:
	ShellScriptCompareCommand(SsdDriverInterface* pDriverInterface,
		shared_ptr<ShellScriptParameterGenInterface> lbaParam,
		shared_ptr<ShellScriptParameterGenInterface> expectedValueParam) {
		mLbaParam = lbaParam;
		mExpectedValueParam = expectedValueParam;
	}

	bool execute() {
		try {
			unsigned int readValue = mpDriverInterface->readSSD((int)mLbaParam->generateParameter());
			return (readValue == mExpectedValueParam->generateParameter());
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