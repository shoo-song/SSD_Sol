#include "shell_exit_command.h"

ShellExitCommand::ShellExitCommand() {}

string ShellExitCommand::execute(vector<string> args) {
    if (args.size() != 1) {
        throw ShellException("args parameter size invalid");
    }
    exit(0);
    return "NA";
}
