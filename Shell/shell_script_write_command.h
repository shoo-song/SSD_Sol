#pragma once
#include <memory>

#include "script_parameter_generator.h"
#include "shell_script_command_interface.h"
#include "shell_util.h"
#include "ssddriver_interface.h"

using std::shared_ptr;

class ShellScriptWriteCommand : public ShellScriptCommandInterface {
   public:
    ShellScriptWriteCommand(SsdDriverInterface* pDriverInterface,
                            shared_ptr<ShellScriptParameterGenInterface> lbaParam,
                            shared_ptr<ShellScriptParameterGenInterface> writeValueParam);
    bool execute();

   private:
    SsdDriverInterface* mpDriverInterface;
    shared_ptr<ShellScriptParameterGenInterface> mLbaParam;
    shared_ptr<ShellScriptParameterGenInterface> mWriteValueParam;
};