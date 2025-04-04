#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;

class ShellScriptCommandInterface {
public:
	virtual bool execute() = 0;
};