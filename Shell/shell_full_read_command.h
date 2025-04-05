#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"

class ShellFullReadCommand : public ShellCommandInterface {
   public:
    ShellFullReadCommand();
    string execute(vector<string> args);

   private:
};