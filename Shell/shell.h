#pragma once
#include <iostream>
#include <string>

#include "script_loader.h"
#include "shell_executor.h"
using std::cin;
using std::cout;
using std::string;

class Shell {
   public:
    Shell();
    int runShell(int argc, char** argv);

   private:
    void runShellMode();
    void runRunnerMode(string runnerScriptName);

    ShellExecutor mExcutor;
    ScriptLoader mScriptLoader;
};