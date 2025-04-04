#pragma once
#include "shell_script_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"
#include "script_parameter_generator.h"
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
			int endLoopCnt = mpEndLoop->generateParameter();
			int cntPerLoop = mpCntPerLoop->generateParameter();

			while (mCurLoopCnt < endLoopCnt) {
				for (shared_ptr<ShellScriptCommandInterface> cmd : mpCommandVec) {
					if (cmd->execute() == false)
						return false;
				}

				mCurLoopCnt += cntPerLoop;
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