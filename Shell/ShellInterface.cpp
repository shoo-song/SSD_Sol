#pragma once
#include "ShellCommandFactory.cpp"
#include "ShellInterfaceUtil.cpp"
#include "ssddriver_interface.h"
#include <sstream>

class ShellInterface {
public:
	void setDriverInterface(SsdDriverInterface* pDriverInterface) {
		mpDriverInterface = pDriverInterface;
		mCommandFactory.setDriverInterface(pDriverInterface);
	}

	string execute(string input) {

		try {
			vector<string> separatedStr = splitString(input);

			ShellCommand cmd = ShellInterfaceUtil::getUtilObj().parse(separatedStr[0]);
			shared_ptr<ShellCommandInterface> commandExecuter = mCommandFactory.getCommand(cmd);

			return commandExecuter->execute(separatedStr);
		}
		catch (ShellArgConvertException e) {
			throw e;
		}
		catch (exception e) {
			throw ShellArgConvertException("invalid args");
		}
	}

private:
	std::vector<std::string> splitString(const std::string& s) {
		std::vector<std::string> result;
		std::istringstream iss(s);
		std::string word;

		while (iss >> word) { // 공백과 탭을 자동으로 구분
			result.push_back(word);
		}

		return result;
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

	SsdDriverInterface* mpDriverInterface;
	ShellCommandFactory mCommandFactory;
};