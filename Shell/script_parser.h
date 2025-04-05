
#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "script_parameter_generator.h"
#include "shell_script_command_interface.h"
#include "shell_script_compare_command.h"
#include "shell_script_loop_command.h"
#include "shell_script_write_command.h"
#include "shell_script_erase_command.h"
#include "shell_util.h"
#include <stack>
using std::make_shared;
using std::shared_ptr;

class ScriptParser {
   public:
    std::vector<shared_ptr<ShellScriptCommandInterface>> makingScript(string scriptName);

   private:
    std::string trim(const std::string& s);
    // 파라미터 토큰을 파싱하는 함수
    shared_ptr<ShellScriptParameterGenInterface> parseParameter(
        const std::string& token, shared_ptr<ShellScriptLoopIdxGetter> looper);
    // 재귀적으로 스크립트 라인을 파싱
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(
        shared_ptr<ShellScriptLoopIdxGetter> parentLooper, const std::vector<std::string>& lines,
        size_t& idx);
    void validationStackClear();
    std::stack<char> mValidationStack;
};
