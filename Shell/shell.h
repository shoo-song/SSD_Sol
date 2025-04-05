#pragma once
#include <string>

using std::string;

class Shell {
   public:
    Shell();
    int runShell(int argc, char** argv);

   private:
    void runShellMode();
    void runRunnerMode(string runnerScriptName);
};