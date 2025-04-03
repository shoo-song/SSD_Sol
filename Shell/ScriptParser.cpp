
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "ShellScriptCommandInterface.h"
#include "ShellScriptLoopCommand.cpp"
#include "ShellScriptCompareCommand.cpp"
#include "ShellScriptWriteCommand.cpp"
#include "ShellUtil.cpp"
using std::shared_ptr;

class ScriptParser {
public:

    std::vector<shared_ptr<ShellScriptCommandInterface>> MakingScript(string scriptName) {
        std::vector<shared_ptr<ShellScriptCommandInterface>> script;

        std::ifstream file(scriptName);
        if (!file) {
            throw std::runtime_error("Error: .");
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        try {
            size_t idx = 0;
            script = parseStatements(lines, idx);
        }
        catch (const std::exception& ex) {
            throw std::runtime_error("Error: Unprocessed extra lines remain in the script.");
        }
        return script;
    }
private:

    // 공백 제거 함수
    std::string trim(const std::string& s) {
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

    // 재귀적으로 스크립트 라인을 파싱하는 함수
    // idx는 현재 라인 인덱스를 가리키며, block 종료('}')를 만나면 반환합니다.
    std::vector<shared_ptr<ShellScriptCommandInterface>> parseStatements(const std::vector<std::string>& lines, size_t& idx) {
        std::vector<shared_ptr<ShellScriptCommandInterface>> commandVec;
        std::regex loopRegex(R"(^loop\s+(\d+)\s*\{\s*$)");  // loop header 패턴

        for (; idx < lines.size(); ++idx) {
            std::string line = trim(lines[idx]);
            if (line.empty())
                continue;

            // 블록 종료를 알리는 '}'를 만나면 이 블록의 파싱 종료
            if (line == "}") {
                // 블록이 정상 종료되었으므로 idx를 한 칸 증가시키고 반환
                ++idx;
                return commandVec;
            }

            std::smatch match;
            if (std::regex_match(line, match, loopRegex)) {
                // loop 블록 시작
                int loopCnt = std::stoi(match[1]);
                ++idx; // loop 다음 줄부터 블록 내부 파싱 시작

                // 재귀 호출하여 현재 loop의 내부 구문 파싱
                shared_ptr<ShellScriptLoopCommand> compositCmd = std::make_shared<ShellScriptLoopCommand>(loopCnt);
                std::vector<shared_ptr<ShellScriptCommandInterface>> childrenCmds = parseStatements(lines, idx);

                for (shared_ptr<ShellScriptCommandInterface> scriptCommand : childrenCmds) {
                    compositCmd->addCommand(scriptCommand);
                }
                commandVec.push_back(compositCmd);
            }
            else {
                // loop header가 아닌 경우는 단순 명령어로 처리 (닫는 중괄호 '}'도 여기서 걸러짐)
                // 만약 line이 "loop"로 시작하지만 형식이 올바르지 않으면 예외 처리
                if (line.find("loop") == 0) {
                    throw std::runtime_error("Error: Malformed loop statement or missing '{' at line: " + line);
                }
                // 명령어 파싱: 공백으로 분리
                vector<string> separatedStr = ShellUtil::getUtilObj().splitString(line);
                ShellScriptApiCommand scriptCmdEnum = ShellUtil::getUtilObj().parseScriptApiCmd(separatedStr[0]);
                shared_ptr<ShellScriptCommandInterface> scriptCmd;

                if (scriptCmdEnum == SCRIPT_UNKOWN) {
                    throw std::runtime_error("unkown cmd");
                }
                else if (scriptCmdEnum == COMPARE_SCRIPT_COMMAND) {
                    scriptCmd = std::make_shared<ShellScriptCompareCommand>(nullptr, separatedStr);
                }
                else if (scriptCmdEnum == WRITE_SCRIPT_COMMAND) {
                    scriptCmd = std::make_shared<ShellScriptWriteCommand>(nullptr, separatedStr);
                }
                commandVec.push_back(scriptCmd);
            }
        }

        return commandVec;
    }
};

//이중루프, GenerateLBA, GenerateData 클래스 고려