#include "shell_help_command.h"

ShellHelpCommand::ShellHelpCommand() {
}

string ShellHelpCommand::execute(vector<string> args) {
	if (args.size() != 1) {
		throw ShellArgConvertException("args parameter size invalid");
	}
	string output = "Team:Alpha Devs(ÀÌ¿øÃ¶/¼Û½ÂÈ£/½Åµ¿Àç/ÀüºÀ¼ö)\n"
		"READ command: read [LBA]\n"
		"Write command: write [LBA] [DATA:(ex)0x123456]\n"
		"Full Read command: fullread\n"
		"Full Write command: fullwrite\n"
		"Exit command: exit\n";

	output += "\n\n SCRIPT \n\n";
	vector<string>& scriptNames = ScriptStore::getScriptStore().getScriptNames();
	for (string scriptName : scriptNames) {
		output += scriptName + "\n";
	}
	return output;
}