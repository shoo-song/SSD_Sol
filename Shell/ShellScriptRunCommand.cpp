#pragma once
#include "ShellException.cpp"
#include "ShellUtil.cpp"
#include "ShellCommandInterface.h"
#include "ShellScriptCommandInterface.h"
#include <memory>

using std::shared_ptr;

class ShellScriptRunnerCommand : public ShellCommandInterface {
public:
	ShellScriptRunnerCommand(vector<shared_ptr< ShellScriptCommandInterface>> ptr) {
		mpScriptCmd = ptr;
	}

	string execute(vector<string> args) {
		try {

			for (shared_ptr< ShellScriptCommandInterface> scriptCmd: mpScriptCmd) {
				if (scriptCmd->execute() != false) {
					return "FAIL";
				}
			}

			return "PASS";
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}
private:
	vector<shared_ptr< ShellScriptCommandInterface>> mpScriptCmd;
};