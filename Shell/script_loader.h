#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>
#include <exception>
#include "script_parser.h"
#include "ssddriver_interface.h"
#include "script_store.h"

namespace fs = std::filesystem;

using std::vector;
using std::string;
using std::exception;

class ScriptLoader {
public:
    void loadScript(SsdDriverInterface* pDriverInterface);
private:
    vector<string> getScriptFileName();
};