#pragma once
#include "ShellExecutor.cpp"
#include "ScriptLoader.cpp"
#include "ShellScriptRunnerMode.cpp"
#include "ssddriver.h"
#include <iostream>
#include <string>
using std::string;
using std::cin;
using std::cout;

class Shell {
public:
    Shell() {
        mExcutor.setDriverInterface(&mSsdDriver);
        mScriptLoader.loadScript(&mSsdDriver);
    }
    void runShell(int argc, char** argv) {
        if (argc == 1) {
            runShellMode();
        }
        else if(argc == 2) {
            mRunnerMode.runRunnerMode(argv[1], &mExcutor);
        }
        else {
            // err
            cout << "Invalid parameter \n";
        }
    }
private:
    void runShellMode() {
        while (true) {
            try {
                string input;
                cout << "Shell > ";
                std::getline(std::cin, input);

                cout << mExcutor.execute(input) << "\n";
            }
            catch (exception e) {
                std::cout << "INVALID COMMAND\n";
            }
        }
    }

    ShellScriptRunnerMode mRunnerMode;
    ShellExecutor mExcutor;
    SsdDriver mSsdDriver; 
    ScriptLoader mScriptLoader;


};