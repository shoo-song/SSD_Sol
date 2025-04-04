#pragma once
#include <memory>

#include "script_parameter_generator.h"
#include "script_store.h"
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_script_command_interface.h"
#include "shell_util.h"

using std::shared_ptr;

class ShellScriptRunnerCommand : public ShellCommandInterface {
   public:
    ShellScriptRunnerCommand();
    string execute(vector<string> args);

   private:
};