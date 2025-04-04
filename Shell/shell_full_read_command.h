#pragma once
#include "shell_command_interface.h"
#include "shell_exception.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

class ShellFullReadCommand : public ShellCommandInterface {
   public:
    ShellFullReadCommand(SsdDriverInterface* pDriverInterface);
    string execute(vector<string> args);

   private:
    SsdDriverInterface* mpDriverInterface;
};