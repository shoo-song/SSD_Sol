#pragma once
#include <memory>

#include "script_parameter_generator.h"
#include "shell_script_command_interface.h"

using std::shared_ptr;

class ShellScriptLoopCommand : public ShellScriptCommandInterface, public ShellScriptLoopIdxGetter {
   public:
    ShellScriptLoopCommand(shared_ptr<ShellScriptParameterGenInterface> startParameter,
                           shared_ptr<ShellScriptParameterGenInterface> endParameter,
                           shared_ptr<ShellScriptParameterGenInterface> cntPerLoop);
    void addCommand(shared_ptr<ShellScriptCommandInterface> command);
    int getCurrentLoopIdx();
    bool execute();

   private:
    vector<shared_ptr<ShellScriptCommandInterface>> mpCommandVec;
    int mCurLoopCnt;
    shared_ptr<ShellScriptParameterGenInterface> mpStartLoop;
    shared_ptr<ShellScriptParameterGenInterface> mpEndLoop;
    shared_ptr<ShellScriptParameterGenInterface> mpCntPerLoop;
};