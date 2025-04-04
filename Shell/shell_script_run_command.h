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
	ShellScriptRunnerCommand();
	string execute(vector<string> args);
private:
};