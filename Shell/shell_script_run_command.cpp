#include "shell_script_run_command.h"

ShellScriptRunnerCommand::ShellScriptRunnerCommand() {}

string ShellScriptRunnerCommand::execute(vector<string> args) {
    try {
        if (args.size() != 1) {
            throw ShellException("invalid args");
        }
        if (ScriptStore::getScriptStore().isThereScript(args[0]) == false) {
            throw ShellException("invalid args");
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
    } catch (ShellException e) {
        throw e;
    } catch (exception e) {
        throw ShellException("invalid args");
    }
}