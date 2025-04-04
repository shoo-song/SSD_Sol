#pragma once
#include "ShellExecutor.cpp"
#include "ScriptLoader.cpp"
#include "ShellScriptRunnerMode.cpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using std::string;
using std::cin;
using std::cout;

class ShellScriptRunnerMode {
public:
    ShellScriptRunnerMode() {
    }
    void runRunnerMode(string runnerScriptName, ShellExecutor* pExecutor) {
        try {
            std::ifstream file(runnerScriptName);
            if (!file) {
                throw std::runtime_error("Error: .");
            }
            string line;
            while (std::getline(file, line)) {
                cout << line << " ______ Run ...";
                if (pExecutor->execute(line).compare("PASS") == 0) {
                    cout << "PASS\n";
                }
                else {
                    cout << "FAIL\n";
                    return;
                }
            }
        }
        catch (exception e) {
            cout << "FAIL\n";
        }
    }
private:


};