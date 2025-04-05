#pragma once
#include <memory>

#include "shell_script_command_interface.h"
using std::shared_ptr;

class ShellScriptFlushCommand : public ShellScriptCommandInterface {
public:
    ShellScriptFlushCommand();
    bool execute();
};
