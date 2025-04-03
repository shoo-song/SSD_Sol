#include <iostream>
#include <io.h>
#include <iomanip>
#include "command_parser.h"
#include <string>

using namespace std;

void CommandParser::PrintError() {
	if (FileObj == nullptr) return;
	FileObj->writeInvalidLog();
}

bool CommandParser::checkInvalidity(const char& CMD, string LBAstring, char* data) {
	if (!((CMD == 'W') || (CMD == 'w') || (CMD == 'R') || (CMD == 'r') || (CMD == 'e') || (CMD == 'E'))) {
		PrintError();
		return false;
	}
	size_t pos;
	uint32_t LBA = std::stoi(LBAstring, &pos, 10);
	if (pos != LBAstring.length()) {
		PrintError();
		return false;
	}
	if (LBA < 0 || LBA > MAX_LBA) {
		PrintError();
		return false;
	}
	if ((CMD == 'W') || (CMD == 'w')) {
		std::string firstTwo(data, 2);
		if (strlen(data) != 10 || firstTwo != "0x") {
			PrintError();
			return false;
		}
		LBA = std::stoul(data, &pos, 16);
		if (pos != strlen(data)) {
			PrintError();
			return false;
		}
	}
	else if ((CMD == 'E') || (CMD == 'e')) {
		int EraseCount = stoi(data);
		if ((EraseCount < 0) || (EraseCount > MAX_ERASE_SIZE)) {
			PrintError();
			return false;
		}
		else if ((LBA + EraseCount) > MAX_LBA) {
			PrintError();
			return false;
		}
	}
	return true;
}

bool CommandParser::parseArg(char CMD, string LBAstring, char* data) {
	if (checkInvalidity(CMD, LBAstring, data) != true) return false;
	InputCMD.LBA = stoi(LBAstring);
	if ((CMD == 'W') || (CMD == 'w')) {
		strcpy_s(InputCMD.input_data, data);
		InputCMD.CMDType = CMD_WRITE;
	}
	else if ((CMD == 'R') || (CMD == 'r')) {
		InputCMD.CMDType = CMD_READ;
	}
	else {
		InputCMD.EraseEndLBA = InputCMD.LBA + stoi(data)-1;
		InputCMD.CMDType = CMD_ERASE;
	}
	return true;
}
