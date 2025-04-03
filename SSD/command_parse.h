#pragma once
#include "FileManager.h"
#define CMD_WRITE (0)
#define CMD_READ (1)
#define CMD_ERASE (2)
#define MAX_LBA (99)
#define MAX_ERASE_SIZE (10)
class SSDCommand {
public:
	SSDCommand() : FileObj(std::make_unique<FileManager>()) {
	}

	void WriteInvalidLog();

	bool checkInvalidity(const char& CMD, string LBAstring, char* data);

	bool parseArg(char CMD, string LBAstring, char* data = NULL);

	int GetCmdType(void) {
		return CMDType;
	}
	int GetLBA(void) {
		return LBA;
	}
	int GetEraseCount(void) {
		return EraseCount;
	}
	char* GetData(void) {
		return input_data;
	}
private:
	int CMDType = 0;
	int LBA = 0;
	int EraseCount = 0;
	char input_data[20] = {};
	std::unique_ptr<FileManager> FileObj;
};
