#pragma once
#include "shell_executor.h"
#include "script_loader.h"
#include "ssddriver.h"
#include <iostream>
#include <string>
using std::string;
using std::cin;
using std::cout;

class Shell {
public:
    Shell();
    void runShell(int argc, char** argv);
private:
    void runShellMode();
    void runRunnerMode(string runnerScriptName);

    ShellExecutor mExcutor;
    SsdDriver mSsdDriver; 
    ScriptLoader mScriptLoader;


};