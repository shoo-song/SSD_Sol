#pragma once
#include <sstream>

#include "shell_command_factory.h"
#include "shell_util.h"

class ShellExecutor {
   public:
    string execute(string input, bool isRunnerMode);

   private:
    ShellCommandFactory mCommandFactory;
};