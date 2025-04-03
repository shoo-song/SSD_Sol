#include <iostream>
#include <io.h>
#include <iomanip>
#include "command_parse.h"
#include <string>

using namespace std;

void SSDCommand::WriteInvalidLog() {
	if (FileObj == nullptr) return;
	FileObj->writeInvalidLog();
}

bool SSDCommand::checkInvalidity(const char& CMD, string LBAstring, char* data) {
	if (!((CMD == 'W') || (CMD == 'w') || (CMD == 'R') || (CMD == 'r') || (CMD == 'e') || (CMD == 'E'))) {
		WriteInvalidLog();
		return false;
	}
	size_t pos;
	uint32_t LBA = std::stoi(LBAstring, &pos, 10);
	if (pos != LBAstring.length()) {
		WriteInvalidLog();
		return false;
	}
	if (LBA < 0 || LBA > MAX_LBA) {
		WriteInvalidLog();
		return false;
	}
	if ((CMD == 'W') || (CMD == 'w')) {
		std::string firstTwo(data, 2);
		if (strlen(data) != 10 || firstTwo != "0x") {
			WriteInvalidLog();
			return false;
		}
		LBA = std::stoul(data, &pos, 16);
		if (pos != strlen(data)) {
			WriteInvalidLog();
			return false;
		}
	}
	else if ((CMD == 'E') || (CMD == 'e')) {
		int EraseCount = stoi(data);
		if ((EraseCount < 0) || (EraseCount > MAX_ERASE_SIZE)) {
			WriteInvalidLog();
			return false;
		}
		else if ((LBA + EraseCount) > MAX_LBA) {
			WriteInvalidLog();
			return false;
		}
	}
	return true;
}

bool SSDCommand::parseArg(char CMD, string LBAstring, char* data) {
	if (checkInvalidity(CMD, LBAstring, data) != true) return false;

	if ((CMD == 'W') || (CMD == 'w')) {
		strcpy_s(input_data, data);
		CMDType = CMD_WRITE;
	}
	else if ((CMD == 'R') || (CMD == 'r')) {
		CMDType = CMD_READ;
	}
	else {
		EraseCount = stoi(data);
		CMDType = CMD_ERASE;
	}
	return true;
}
