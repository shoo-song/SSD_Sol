#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;

class ShellCommandInterface {
public:
	virtual vector<unsigned int> convertCmdArgs(vector<string> args) = 0;
};