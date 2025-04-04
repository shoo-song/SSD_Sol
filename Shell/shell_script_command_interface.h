#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class ShellScriptCommandInterface {
   public:
    virtual bool execute() = 0;
};