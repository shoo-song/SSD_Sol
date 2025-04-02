#pragma once
#include "ShellCommandInterface.h"
#include "ShellException.cpp"

class ShellReadCommand: public ShellCommandInterface {
public:
	vector<unsigned int>  convertCmdArgs(vector<string> args) {
		try {
			vector<unsigned int> output;

			if (args.size() != 1) {
				throw ShellArgConvertException("args parameter size invalid");
			}

			output.push_back(convertDecimalStringForLba(args[0]));
			return output;
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}
private:
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
};