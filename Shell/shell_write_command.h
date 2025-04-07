#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"

class ShellWriteCommand : public ShellCommandInterface {
   public:
    ShellWriteCommand();
    string execute(vector<string> args);

   private:
    vector<unsigned int> convertCmdArgs(vector<string> args);
};