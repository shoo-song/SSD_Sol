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

bool CommandParser::checkInvalidity(int argCount, const char& CMD, string LBAstring, char* data) {
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
	if (LBA < 0 || LBA >= MAX_LBA_COUNT) {
		PrintError();
		return false;
	}
	if ((CMD == 'W') || (CMD == 'w')) {
		if (argCount != 4) {
			PrintError();
			return false;
		}
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
		if (argCount != 4) {
			PrintError();
			return false;
		}
		int EraseCount = stoi(data);
		if ((EraseCount < 0) || (EraseCount > MAX_ERASE_SIZE)) {
			PrintError();
			return false;
		}
		else if ((LBA + EraseCount) > MAX_LBA_COUNT) {
			PrintError();
			return false;
		}
	}
	return true;
}

CmdInfo CommandParser::parseArg(int argCount, char CMD, string LBAstring, char* data) {
	CmdInfo Command;
	Command.IsValid = false;
	Command.LBAString = LBAstring;
	if (checkInvalidity(argCount, CMD, LBAstring, data) != true) {
		return Command;
	}
	Command.LBA = stoi(LBAstring);
	if ((CMD == 'W') || (CMD == 'w')) {
		strcpy_s(Command.input_data, data);
		Command.CMDType = CMD_WRITE;
	}
	else if ((CMD == 'R') || (CMD == 'r')) {
		Command.CMDType = CMD_READ;
	}
	else if ((CMD == 'E') || (CMD == 'e')) {
		Command.EraseEndLBA = Command.LBA + stoi(data)-1;
		strcpy_s(Command.input_data, data);
		Command.CMDType = CMD_ERASE;
	}
	else {
		return Command;
	}
	Command.IsValid = true;
	return Command;
}
