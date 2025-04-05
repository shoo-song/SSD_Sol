#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"

class ShellFullWriteCommand : public ShellCommandInterface {
   public:
    ShellFullWriteCommand();
    string execute(vector<string> args);

   private:
    vector<unsigned int> convertCmdArgs(vector<string> args);
};