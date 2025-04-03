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
	if (!((CMD == 'W') || (CMD == 'w') || (CMD == 'R') || (CMD == 'r'))) {
		WriteInvalidLog();
		return false;
	}

	size_t pos;
	uint32_t value = std::stoi(LBAstring, &pos, 10);
	if (pos != LBAstring.length()) {
		WriteInvalidLog();
		return false;
	}
	if (value < 0 || value > 99) {
		WriteInvalidLog();
		return false;
	}
	if ((CMD == 'W') || (CMD == 'w')) {
		std::string firstTwo(data, 2);
		if (strlen(data) != 10 || firstTwo != "0x") {
			WriteInvalidLog();
			return false;
		}
		value = std::stoul(data, &pos, 16);
		if (pos != strlen(data)) {
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
		bIsWrite = true;
	}
	else if ((CMD == 'R') || (CMD == 'r')) {
		bIsWrite = false;
	}
	LBA = std::stoi(LBAstring);	
	return true;
}
