#pragma once
#include "DataFileSystem.h"
#define CMD_WRITE ('W')
#define CMD_READ ('R')
#define CMD_ERASE ('E')
#define CMD_FLUSH ('F')
#define MAX_LBA_COUNT (100)
#define MAX_ERASE_SIZE (10)
using namespace std;
struct CmdInfo {
	char CMDType;
	int LBA;
	string LBAString;
	char input_data[20];
	int EraseEndLBA = 0;
	bool IsValid = false;
};
class CommandParser {
public:
	CommandParser() : FileObj(std::make_unique<DataFileSystem>()) {
	}

	void PrintError();

	bool checkInvalidity(int argCount, const char& CMD, string LBAstring, char* data);
	string toTwoDigitString(unsigned int value);
	CmdInfo parseArg(int argCount, char CMD, string LBAstring, char* data = NULL);
private:
	std::unique_ptr<DataFileSystem> FileObj;
};
