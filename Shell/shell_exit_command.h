#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"

class ShellExitCommand : public ShellCommandInterface {
   public:
    ShellExitCommand();
    string execute(vector<string> args);

   private:
};