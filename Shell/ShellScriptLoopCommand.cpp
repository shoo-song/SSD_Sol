#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
#include "ScriptParameterGenerator.h"
#include <memory>

using std::shared_ptr;

class ShellScriptLoopCommand : public ShellScriptCommandInterface, public ShellScriptLoopIdxGetter {
public:
	ShellScriptLoopCommand(shared_ptr<ShellScriptParameterGenInterface> startParameter,
		shared_ptr<ShellScriptParameterGenInterface> endParameter,
		shared_ptr<ShellScriptParameterGenInterface> cntPerLoop) {
		mpStartLoop = startParameter;
		mpEndLoop = endParameter;
		mpCntPerLoop = cntPerLoop;
		mCurLoopCnt = 0;
	}

	void addCommand(shared_ptr<ShellScriptCommandInterface> command) {
		mpCommandVec.push_back(command);
	}

	int getCurrentLoopIdx() {
		return mCurLoopCnt;
	}

	bool execute() {
		try {
			mCurLoopCnt = mpStartLoop->generateParameter();

			while (mCurLoopCnt < mpEndLoop->generateParameter()) {
				for (shared_ptr<ShellScriptCommandInterface> cmd : mpCommandVec) {
					if (cmd->execute() == false)
						return false;
				}

				mCurLoopCnt += mpCntPerLoop->generateParameter();
			}
			return true;
		}
		catch (exception e) {
			return false;
		}
	}
private:
	vector<shared_ptr< ShellScriptCommandInterface>> mpCommandVec;
	int mCurLoopCnt;
	shared_ptr<ShellScriptParameterGenInterface> mpStartLoop;
	shared_ptr<ShellScriptParameterGenInterface> mpEndLoop;
	shared_ptr<ShellScriptParameterGenInterface> mpCntPerLoop;
};