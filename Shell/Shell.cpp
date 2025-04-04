#pragma once
#include "ShellExecutor.cpp"
#include "ScriptLoader.cpp"
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
    void runShell() {
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
private:
    ShellExecutor mExcutor;
    SsdDriver mSsdDriver; 
    ScriptLoader mScriptLoader;


};