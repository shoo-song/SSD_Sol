#include "shell_script_run_command.h"

ShellScriptRunnerCommand::ShellScriptRunnerCommand() {}

string ShellScriptRunnerCommand::execute(vector<string> args) {
    try {
        if (args.size() != 1) {
            throw ShellArgConvertException("invalid args");
        }
        if (ScriptStore::getScriptStore().isThereScript(args[0]) == false) {
            throw ShellArgConvertException("invalid args");
        }

        ShellScriptRandValStore::getShellScriptRandValStore().clear();

        vector<shared_ptr<ShellScriptCommandInterface>> scriptCmds =
            ScriptStore::getScriptStore().getScriptCommand(args[0]);
        for (shared_ptr<ShellScriptCommandInterface> scriptCmd : scriptCmds) {
            if (scriptCmd->execute() == false) {
                return "FAIL";
            }
        }

        return "PASS";
    } catch (ShellArgConvertException e) {
        throw e;
    } catch (exception e) {
        throw ShellArgConvertException("invalid args");
    }
}