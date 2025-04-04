#pragma once
#include "shell_exception.h"
#include "shell_util.h"
#include "shell_command_interface.h"
#include "shell_script_command_interface.h"
#include "script_store.h"
#include "script_parameter_generator.h"
#include <memory>

using std::shared_ptr;

class ShellScriptRunnerCommand : public ShellCommandInterface {
public:
	ShellScriptRunnerCommand() {
	}

	string execute(vector<string> args) {
		try {
			if (args.size() != 1) {
				throw ShellArgConvertException("invalid args");
			}
			if (ScriptStore::getScriptStore().isThereScript(args[0]) == false) {
				throw ShellArgConvertException("invalid args");
			}

			ShellScriptRandValStore::getShellScriptRandValStore().clear();

			vector<shared_ptr<ShellScriptCommandInterface>> scriptCmds = ScriptStore::getScriptStore().getScriptCommand(args[0]);
			for (shared_ptr< ShellScriptCommandInterface> scriptCmd: scriptCmds) {
				if (scriptCmd->execute() == false) {
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
};