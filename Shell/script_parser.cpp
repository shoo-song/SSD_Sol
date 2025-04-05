#include "script_parser.h"
#include "shell_script_compare_command.h"
#include "shell_script_loop_command.h"
#include "shell_script_write_command.h"
#include "shell_script_erase_command.h"
#include "shell_script_flush_command.h"
#include "shell_util.h"
#include <regex>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <iostream>

std::vector<shared_ptr<ShellScriptCommandInterface>> ScriptParser::makingScript(string scriptName) {
    std::vector<shared_ptr<ShellScriptCommandInterface>> script;

    std::ifstream file(scriptName);
    if (!file) {
        throw std::runtime_error("Error: .");
    }

    validationStackClear();
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    try {
        size_t idx = 0;
        script = parseStatements(nullptr, lines, idx);
        if (!mValidationStack.empty()) {
            throw std::runtime_error("Error: .");
        }
    } catch (const std::exception& ex) {
        throw ex;
    }
    return script;
}

void ScriptParser::validationStackClear() {
    while (!mValidationStack.empty()) {
        mValidationStack.pop();
    }
}

// 공백 제거 함수
std::string ScriptParser::trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

// 파라미터 토큰을 파싱하는 함수
shared_ptr<ShellScriptParameterGenInterface> ScriptParser::parseParameter(
    const std::string& token, shared_ptr<ShellScriptLoopIdxGetter> looper) {
    // VAL(숫자) : 10진수 혹은 16진수
    std::regex regexVal(R"(VAL\(\s*((?:0[xX][0-9a-fA-F]+)|\d+)\s*\))");

    // IND(숫자) 또는 IND() : 숫자 없으면 기본 0, 음수 포함
    std::regex regexInd(R"(IND\(\s*(-?(?:(?:0[xX][0-9a-fA-F]+)|\d+))?\s*\))");

    // RAND(숫자,숫자,숫자) : 각 숫자가 10진수 또는 16진수
    std::regex regexRand(R"(RAND\(\s*((?:0[xX][0-9a-fA-F]+)|\d+)\s*,\s*((?:0[xX][0-9a-fA-F]+)|\d+)\s*,\s*((?:0[xX][0-9a-fA-F]+)|\d+)\s*\))");

    // RAND_REF(숫자) : 10진수 또는 16진수
    std::regex regexRandRef(R"(RAND_REF\(\s*((?:0[xX][0-9a-fA-F]+)|\d+)\s*\))");

    std::smatch match;

    if (std::regex_match(token, match, regexVal)) {
        return make_shared<ScriptParamValGen>(std::stoul(match[1], nullptr, 0));
    } else if (std::regex_match(token, match, regexInd)) {
        if (looper == nullptr) {
            throw std::runtime_error("looper 0");
        }
        return make_shared<ScriptParamIdxGen>(looper, std::stoi(match[1], nullptr, 0));
    } else if (std::regex_match(token, match, regexRand)) {
        return make_shared<ScriptParamRanGen>(
            std::stoi(match[1], nullptr, 0),
            std::stoi(match[2], nullptr, 0),
            std::stoul(match[3], nullptr, 0)
        );
    } else if (std::regex_match(token, match, regexRandRef)) {
        return make_shared<ShellScriptRandGetGen>(std::stoi(match[1], nullptr, 0));
    }
    return nullptr;
}

// 재귀적으로 스크립트 라인을 파싱
// idx는 현재 라인 인덱스를 가리키며, block 종료('}')를 만나면 반환
std::vector<shared_ptr<ShellScriptCommandInterface>> ScriptParser::parseStatements(
    shared_ptr<ShellScriptLoopIdxGetter> parentLooper, const std::vector<std::string>& lines,
    size_t& idx) {
    std::vector<shared_ptr<ShellScriptCommandInterface>> commandVec;
    std::regex loopRegex(R"(^loop\s+(\S+)\s+(\S+)\s+(\S+)\s*\{$)");  // loop header 패턴

    for (; idx < lines.size(); ++idx) {
        std::string line = trim(lines[idx]);
        if (line.empty())
            continue;

        // 블록 종료를 알리는 '}'를 만나면 이 블록의 파싱 종료
        if (line == "}") {
            if (mValidationStack.empty()) {
                throw std::runtime_error("Error: .");
            }
            mValidationStack.pop();
            return commandVec;
        }

        std::smatch match;
        if (std::regex_match(line, match, loopRegex)) {
            // loop 블록 시작
            shared_ptr<ShellScriptParameterGenInterface> loopStart =
                parseParameter(match[1].str(), parentLooper);
            shared_ptr<ShellScriptParameterGenInterface> loopEnd =
                parseParameter(match[2].str(), parentLooper);
            shared_ptr<ShellScriptParameterGenInterface> loopIncrement =
                parseParameter(match[3].str(), parentLooper);

            ++idx;  // loop 다음 줄부터 블록 내부 파싱 시작
            mValidationStack.push('{');

            // 재귀 호출하여 현재 loop의 내부 구문 파싱
            shared_ptr<ShellScriptLoopCommand> compositCmd =
                std::make_shared<ShellScriptLoopCommand>(loopStart, loopEnd, loopIncrement);
            std::vector<shared_ptr<ShellScriptCommandInterface>> childrenCmds =
                parseStatements(compositCmd, lines, idx);

            for (shared_ptr<ShellScriptCommandInterface> scriptCommand : childrenCmds) {
                compositCmd->addCommand(scriptCommand);
            }
            commandVec.push_back(compositCmd);
        } else {
            // 명령어 파싱: 공백으로 분리. Parameter는 
            vector<string> separatedStr = ShellUtil::getUtilObj().splitString(line);
            ShellScriptApiCommand scriptCmdEnum =
                ShellUtil::getUtilObj().parseScriptApiCmd(separatedStr[0]);
            shared_ptr<ShellScriptCommandInterface> scriptCmd;

            if (scriptCmdEnum == SCRIPT_UNKOWN) {
                throw std::runtime_error("unkown cmd");
            } else if (scriptCmdEnum == COMPARE_SCRIPT_COMMAND) {
                shared_ptr<ShellScriptParameterGenInterface> lba =
                    parseParameter(separatedStr[1], parentLooper);
                shared_ptr<ShellScriptParameterGenInterface> expected =
                    parseParameter(separatedStr[2], parentLooper);

                scriptCmd =
                    std::make_shared<ShellScriptCompareCommand>(lba, expected);
            } else if (scriptCmdEnum == WRITE_SCRIPT_COMMAND) {
                shared_ptr<ShellScriptParameterGenInterface> lba =
                    parseParameter(separatedStr[1], parentLooper);
                shared_ptr<ShellScriptParameterGenInterface> val =
                    parseParameter(separatedStr[2], parentLooper);

                scriptCmd = std::make_shared<ShellScriptWriteCommand>(lba, val);
            } else if (scriptCmdEnum == ERASE_SCRIPT_COMMAND) {
                shared_ptr<ShellScriptParameterGenInterface> startLba =
                    parseParameter(separatedStr[1], parentLooper);
                shared_ptr<ShellScriptParameterGenInterface> endLba =
                    parseParameter(separatedStr[2], parentLooper);

                scriptCmd =
                    std::make_shared<ShellScriptEraseCommand>(startLba, endLba);
            } else if (scriptCmdEnum == FLUSH_SCRIPT_COMMAND) {
                scriptCmd =
                    std::make_shared<ShellScriptFlushCommand>();
            }
            commandVec.push_back(scriptCmd);
        }
    }
    return commandVec;
}