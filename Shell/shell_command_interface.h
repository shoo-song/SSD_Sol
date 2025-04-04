#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;

class ShellCommandInterface {
public:
	virtual string execute(vector<string> args) = 0;
};