#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"

class ShellEraseRangeCommand : public ShellCommandInterface {
   public:
    ShellEraseRangeCommand();
    string execute(vector<string> args);

   private:
    vector<unsigned int> convertCmdArgs(vector<string> args);
};