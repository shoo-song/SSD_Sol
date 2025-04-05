
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
using std::make_shared;
using std::shared_ptr;

class ScriptParser {
   public:
    void setSsdDriver(SsdDriverInterface* pdriverInterface);
    std::vector<shared_ptr<ShellScriptCommandInterface>> makingScript(string scriptName);

   private:
    // 공백 제거 함수
    std::string trim(const std::string& s);

    // 파라미터 토큰을 파싱하는 함수
    shared_ptr<ShellScriptParameterGenInterface> parseParameter(
        const std::string& token, shared_ptr<ShellScriptLoopIdxGetter> looper);

    // 재귀적으로 스크립트 라인을 파싱하는 함수
    // idx는 현재 라인 인덱스를 가리키며, block 종료('}')를 만나면 반환합니다.
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(
        shared_ptr<ShellScriptLoopIdxGetter> parentLooper, const std::vector<std::string>& lines,
        size_t& idx);

    SsdDriverInterface* mpDriverInterface;
};

// 이중루프, GenerateLBA, GenerateData 클래스 고려