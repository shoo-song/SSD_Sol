
#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "common.h"
#include "shell_exception.h"

using std::vector;

class ShellUtil {
   public:
    static ShellUtil& getUtilObj();
    ShellScriptApiCommand parseScriptApiCmd(const string input);
    ShellCommand parse(string commandArg);
    // LBA 문자열 변환 (10진수, 0~99)
    unsigned int convertDecimalStringForLba(const std::string& input);
    // Size 문자열 변환 (10진수, 0~100)
    unsigned int convertStrForSize(const std::string& LBA1, const std::string& size);
    unsigned int convertLBAtoSize(const std::string& LBA1, const std::string& LBA2);
    // Data 문자열 변환 (16진수, "0x" + 8자리)
    unsigned int convertHexStringForData(const std::string& input);
    vector<std::string> splitString(const std::string& s);
    string toTwoDigitString(unsigned int value);
    string toHexFormat(unsigned int value);

   private:
    ShellUtil();
};