#pragma once
#include "ShellScriptCommandInterface.h"
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ssddriver_interface.h"
#include <memory>

using std::shared_ptr;

class ShellScriptLoopCommand : public ShellScriptCommandInterface {
public:
	ShellScriptLoopCommand(int loopCnt) {
		mLoopCnt = loopCnt;
	}

	void addCommand(shared_ptr<ShellScriptCommandInterface> command) {
		mpCommandVec.push_back(command);
	}

	bool execute() {
		try {
			for (int i = 0; i < mLoopCnt; i++) {
				for (shared_ptr<ShellScriptCommandInterface> cmd : mpCommandVec) {
					if (cmd->execute() == false)
						return false;
				}
			}
			return true;
		}
		catch (exception e) {
			return false;
		}
	}
private:
	vector<shared_ptr< ShellScriptCommandInterface>> mpCommandVec;
	int mLoopCnt;
};