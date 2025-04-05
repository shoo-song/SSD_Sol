#pragma once
#include <memory>

#include "script_parameter_generator.h"
#include "shell_exception.h"
#include "shell_script_command_interface.h"
#include "shell_util.h"

using std::shared_ptr;

class ShellScriptCompareCommand : public ShellScriptCommandInterface {
   public:
    ShellScriptCompareCommand(shared_ptr<ShellScriptParameterGenInterface> lbaParam,
                              shared_ptr<ShellScriptParameterGenInterface> expectedValueParam);
    bool execute();

   private:
    shared_ptr<ShellScriptParameterGenInterface> mLbaParam;
    shared_ptr<ShellScriptParameterGenInterface> mExpectedValueParam;
};