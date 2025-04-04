#include "shell.h"

Shell::Shell() {
    mExcutor.setDriverInterface(&mSsdDriver);
    mScriptLoader.loadScript(&mSsdDriver);
}
void Shell::runShell(int argc, char** argv) {
    if (argc == 1) {
        runShellMode();
    } else if (argc == 2) {
        runRunnerMode(argv[1]);
    } else {
        // err
        cout << "Invalid parameter \n";
    }
}

void Shell::runShellMode() {
    while (true) {
        try {
            string input;
            cout << "Shell > ";
            std::getline(std::cin, input);

            cout << mExcutor.execute(input, false) << "\n";
        } catch (exception e) {
            std::cout << "INVALID COMMAND\n";
        }
    }
}

void Shell::runRunnerMode(string runnerScriptName) {
    try {
        std::ifstream file(runnerScriptName);
        if (!file) {
            throw std::runtime_error("Error: .");
        }
        string line;
        while (std::getline(file, line)) {
            cout << line << " ______ Run ...";
            if (mExcutor.execute(line, true).compare("PASS") == 0) {
                cout << "PASS\n";
            } else {
                cout << "FAIL\n";
                return;
            }
        }
    } catch (exception e) {
        cout << "FAIL\n";
    }
}
