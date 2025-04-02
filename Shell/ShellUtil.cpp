
#pragma once

#include "Common.h"
#include "ShellException.cpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using std::vector;

class ShellUtil {
public:
	static ShellUtil& getUtilObj() {
		static ShellUtil shellInterfaceUtil;
		return shellInterfaceUtil;
	}

	ShellCommand parseScript(const string input) {
		// 1번 조건 검사
		if (input == "1_FullWriteAndReadCompare" || input.rfind("1_", 0) == 0) {
			return SCRIPT_1;
		}
		// 2번 조건 검사
		if (input == "2_PartialLBAWrite" || input.rfind("2_", 0) == 0) {
			return SCRIPT_2;
		}
		// 3번 조건 검사
		if (input == "3_WriteReadAging" || input.rfind("3_", 0) == 0) {
			return SCRIPT_3;
		}

		return UNKOWN;  // 매칭되는 조건이 없을 경우
	}

	ShellCommand parse(string commandArg) {
		if (commandArg.compare("read") == 0) {
			return READ_COMMAND;
		}
		if (commandArg.compare("write") == 0) {
			return WRITE_COMMAND;
		}
		if (commandArg.compare("help") == 0) {
			return HELP_COMMAND;
		}
		if (commandArg.compare("exit") == 0) {
			return EXIT_COMMAND;
		}
		if (commandArg.compare("fullwrite") == 0) {
			return FULLWRITE_COMMAND;
		}
		if (commandArg.compare("fullread") == 0) {
			return FULLREAD_COMMAND;
		}
		return UNKOWN;
	}

	// LBA 문자열 변환 (10진수, 0~99)
	unsigned int convertDecimalStringForLba(const std::string& input) {
		size_t pos;
		unsigned int value = std::stoi(input, &pos, 10);

		// 변환된 길이 확인 (예: "12abc" 방지)
		if (pos != input.length()) {
			throw ShellArgConvertException("Invalid characters in input: " + input);
		}

		// 범위 검사
		if (value < 0 || value > 99) {
			throw ShellArgConvertException("Value out of range (0-99): " + input);
		}

		return value;
	}

	// Data 문자열 변환 (16진수, "0x" + 8자리)
	unsigned int convertHexStringForData(const std::string& input) {
		// 길이 체크
		if (input.length() != 10 || input.substr(0, 2) != "0x") {
			throw ShellArgConvertException("Invalid hex format: " + input);
		}

		size_t pos;
		unsigned int value = std::stoul(input, &pos, 16);

		// 변환된 길이 확인 (예: "0x1234567G" 방지)
		if (pos != input.length()) {
			throw ShellArgConvertException("Invalid characters in input: " + input);
		}

		return value;
	}

	vector<std::string> splitString(const std::string& s) {
		std::vector<std::string> result;
		std::istringstream iss(s);
		std::string word;

		while (iss >> word) { // 공백과 탭을 자동으로 구분
			result.push_back(word);
		}

		return result;
	}

	string toTwoDigitString(unsigned int value) {
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << value;
		return oss.str();
	}

	string toHexFormat(unsigned int value) {
		std::ostringstream oss;
		oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value;
		return oss.str();
	}
private:
	ShellUtil() {

	}
};