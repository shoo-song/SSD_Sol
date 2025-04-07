
#include <iostream>
#include "logger.h"
#include <memory>
#include "script_loader.h"
#include "shell.h"
#include "shell_executor.h"
#include "ssddriver_store.h"
#include "ssddriver.h"

using std::cin;
using std::cout;
using std::shared_ptr;
using std::make_shared;

Shell::Shell() {
    ScriptLoader loader;

    logger.setLogFile();
    logger.print("Shell.shell()", "∑Œ±Î Ω√¿€");

    loader.loadScript();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(make_shared<SsdDriver>());
}
int Shell::runShell(int argc, char** argv) {
    if (argc == 1) {
        runShellMode();
    } else if (argc == 2) {
        runRunnerMode(argv[1]);
    } else {
        // err
        cout << "Invalid parameter \n";
    }

    return 0;
}

void Shell::runShellMode() {
    ShellExecutor executor;

    while (true) {
        try {
            string input;
            cout << "Shell > ";
            std::getline(std::cin, input);

            cout << executor.execute(input, false) << "\n";
        } catch (exception e) {
            std::cout << "INVALID COMMAND\n";
        }
    }
}

void Shell::runRunnerMode(string runnerScriptName) {
    ShellExecutor executor;

    try {
        std::ifstream file(runnerScriptName);
        if (!file) {
            throw std::runtime_error("Error: .");
        }
        string line;
        while (std::getline(file, line)) {
            cout << line << " ______ Run ...";
            if (executor.execute(line, true).compare("PASS") == 0) {
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
