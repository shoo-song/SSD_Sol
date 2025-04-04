#include "shell_script_loop_command.h"

ShellScriptLoopCommand::ShellScriptLoopCommand(
    shared_ptr<ShellScriptParameterGenInterface> startParameter,
    shared_ptr<ShellScriptParameterGenInterface> endParameter,
    shared_ptr<ShellScriptParameterGenInterface> cntPerLoop) {
    mpStartLoop = startParameter;
    mpEndLoop = endParameter;
    mpCntPerLoop = cntPerLoop;
    mCurLoopCnt = 0;
}

void ShellScriptLoopCommand::addCommand(shared_ptr<ShellScriptCommandInterface> command) {
    mpCommandVec.push_back(command);
}

int ShellScriptLoopCommand::getCurrentLoopIdx() { return mCurLoopCnt; }

bool ShellScriptLoopCommand::execute() {
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
    } catch (exception e) {
        return false;
    }
}