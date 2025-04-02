#pragma once
#include <string>
#include <vector>
#include <stdexcept>

enum ShellCommand {
	READ_COMMAND,
	WRITE_COMMAND,
	HELP_COMMAND,
	EXIT_COMMAND,
	FULLWRITE_COMMAND,
	FULLREAD_COMMAND,
	UNKOWN
};

using std::string;
using std::vector;
using std::exception;

class ShellArgConvertException : public std::exception {
public:
	explicit ShellArgConvertException(const std::string& msg) : message(msg) {}

	const char* what() const noexcept override {
		return message.c_str();
	}
private:
	string message;
};

class ShellInterface {
public:
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
	vector<int> convertCmdArgs(ShellCommand cmd, vector<string> args) {

		try {
			vector<int> output;

			switch (cmd) {
			case READ_COMMAND:
				if (args.size() != 1) {
					throw ShellArgConvertException("args parameter size invalid");
				}

				output.push_back(convertDecimalStringForLba(args[0]));
			}

			return output;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

private:
	// LBA 문자열 변환 (10진수, 0~99)
	int convertDecimalStringForLba(const std::string& input) {
		size_t pos;
		int value = std::stoi(input, &pos, 10);

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
	 int convertHexStringForData(const std::string& input) {
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
};