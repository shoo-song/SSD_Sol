#pragma once
#include <sstream>

#include "shell_command_factory.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellExecutor {
   public:
    void setDriverInterface(SsdDriverInterface* pDriverInterface);
    string execute(string input, bool isRunnerMode);

   private:
    SsdDriverInterface* mpDriverInterface;
    ShellCommandFactory mCommandFactory;
};