#pragma once
#include <sstream>

#include "shell_command_factory.h"
#include "common.h"

class ShellExecutor {
   public:
    string execute(string input, bool isRunnerMode);

   private:
    ShellCommand parse(string commandArg);
    ShellCommandFactory mCommandFactory;
};