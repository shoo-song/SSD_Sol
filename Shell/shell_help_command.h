#pragma once
#include "script_store.h"
#include "shell_command_interface.h"
#include "shell_exception.h"

class ShellHelpCommand : public ShellCommandInterface {
   public:
    ShellHelpCommand();
    string execute(vector<string> args);

   private:
};