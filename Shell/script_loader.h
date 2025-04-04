#pragma once
#include <exception>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "script_parser.h"
#include "script_store.h"
#include "ssddriver_interface.h"

namespace fs = std::filesystem;

using std::exception;
using std::string;
using std::vector;

class ScriptLoader {
   public:
    void loadScript(SsdDriverInterface* pDriverInterface);

   private:
    vector<string> getScriptFileName();
};