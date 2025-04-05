#include "shell_help_command.h"

ShellHelpCommand::ShellHelpCommand() {}

string ShellHelpCommand::execute(vector<string> args) {
    if (args.size() != 1) {
        throw ShellException("args parameter size invalid");
    }
    string output = "\n\n COMMAND \n\n"
        "READ command: read [LBA]\n"
        "Write command: write [LBA] [DATA:(ex)0x123456]\n"
        "Full Read command: fullread\n"
        "Full Write command: fullwrite [DATA:(ex)0x123456]\n"
        "Erase command: erase [LBA] [SIZE]\n"
        "Erase Range command: erase_range [start LBA] [end LBA]\n"
        "Flush command: flush\n"
        "Exit command: exit\n";

    output += "\n\n SCRIPT \n\n";
    vector<string>& scriptNames = ScriptStore::getScriptStore().getScriptNames();
    for (string scriptName : scriptNames) {
        output += scriptName + "\n";
    }
    output += "\n\nTeam:Alpha Devs(ÀÌ¿øÃ¶/¼Û½ÂÈ£/½Åµ¿Àç/ÀüºÀ¼ö)\n";
    return output;
}