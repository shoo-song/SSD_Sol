#include <iostream>
#include <io.h>
#include <iomanip>
#include "Command_parser.h"
#include <string>
#include <sstream>

using namespace std;

bool CommandParser::PrintError() {
	FileObj->writeInvalidLog();
	return false;
}

bool CommandParser::checkCmdTypeInvalidity(const char& CMD){
	if (!((CMD == 'w') || (CMD == 'W') ||
		(CMD == 'r') || (CMD == 'R') ||
		(CMD == 'e') || (CMD == 'E') ||
		(CMD == 'f') || (CMD == 'F'))) {
		return PrintError();
	}
	return true;
}

bool CommandParser::checkLBAInvalidity(size_t pos, std::string& LBAstring, uint32_t LBA){
	if (pos != LBAstring.length()) {
		return PrintError();
	}
	if (LBA < 0 || LBA >= MAX_LBA_COUNT) {
		return PrintError();
	}
	return true;
}

bool CommandParser::checkEraseSize(int argCount, char* data, uint32_t LBA){	
	if (argCount != 4) {
		return PrintError();
	}
	int EraseCount = stoi(data);
	if ((EraseCount < 0) || (EraseCount > MAX_ERASE_SIZE)) {
		return PrintError();
	}
	else if ((LBA + EraseCount) > MAX_LBA_COUNT) {
		return PrintError();
	}
	return true;
}

bool CommandParser::checkWriteDataInvalidity(int argCount, char* data, size_t& pos)
{
	if (argCount != 4) {
		return PrintError();
	}
	std::string firstTwo(data, 2);
	if (strlen(data) != 10 || firstTwo != "0x") {
		return PrintError();
	}
	uint32_t LBA = std::stoul(data, &pos, 16);
	if (pos != strlen(data)) {
		return PrintError();
	}
	return true;
}

bool CommandParser::checkInvalidity(int argCount, const char& CMD, string LBAstring, char* data) {
	bool retVal = checkCmdTypeInvalidity(CMD);
	if (retVal != true) return false;

	if ((CMD == 'f') || (CMD == 'F')) return true;

	size_t pos;
	uint32_t LBA = std::stoi(LBAstring, &pos, 10);

	if (checkLBAInvalidity(pos, LBAstring, LBA) != true) return false;

	if ((CMD == 'W') || (CMD == 'w')) {
		if (checkWriteDataInvalidity(argCount, data, pos) != true) return false;
	}
	else if ((CMD == 'E') || (CMD == 'e')) {
		if (checkEraseSize(argCount, data, LBA) != true) return false;
	}
	return true;
}
string CommandParser::toTwoDigitString(unsigned int value) {
	ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << value;
	return oss.str();
}
CmdInfo CommandParser::parseArg(int argCount, char CMD, string LBAstring, char* data) {
	CmdInfo Command;
	Command.IsValid = false;
	if (argCount == 2) {
		Command.LBAString = toTwoDigitString(0);
	}
	else 
		Command.LBAString = toTwoDigitString(stoi(LBAstring));

	if (checkInvalidity(argCount, CMD, LBAstring, data) != true) {
		return Command;
	}
	if (!LBAstring.empty())	Command.LBA = stoi(LBAstring);
	if ((CMD == 'W') || (CMD == 'w')) {
		strcpy_s(Command.input_data, data);
		Command.CMDType = CMD_WRITE;
	}
	if ((CMD == 'R') || (CMD == 'r')) {
		Command.CMDType = CMD_READ;
	}
	if ((CMD == 'E') || (CMD == 'e')) {
		Command.EraseEndLBA = Command.LBA + stoi(data)-1;
		strcpy_s(Command.input_data, data);
		Command.CMDType = CMD_ERASE;
	}
	if ((CMD == 'F') || (CMD == 'f')) {
		Command.CMDType = CMD_FLUSH;
	}

	Command.IsValid = true;
	return Command;
}
