#pragma once
#include <unordered_map>
#include <memory>
#include "shell_exception.h"
#include "shell_script_command_interface.h"

using std::unordered_map;
using std::shared_ptr;

class ScriptStore {
public:
	static ScriptStore& getScriptStore();
	bool isThereScript(string scriptName);
	vector<string>& getScriptNames();
	vector<shared_ptr<ShellScriptCommandInterface>> getScriptCommand(string scriptName);
	void addScript(string scriptName, vector<shared_ptr<ShellScriptCommandInterface>> command);
private:
	ScriptStore();
	int getWildcardNumber(string fileName);
	vector<std::string> mFileNames;
	unordered_map<int, vector<shared_ptr<ShellScriptCommandInterface>>> mWildCardCommandMap;
	unordered_map<string, vector<shared_ptr<ShellScriptCommandInterface>>> mCommandMap;

};