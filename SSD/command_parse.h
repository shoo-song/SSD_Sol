#pragma once
#include "FileManager.h"

class SSDCommand {
public:
	SSDCommand() : FileObj(std::make_unique<FileManager>()) {
	}

	void WriteInvalidLog();

	bool checkInvalidity(const char& CMD, string LBAstring, char* data);

	bool parseArg(char CMD, string LBAstring, char* data = NULL);

	bool IsWrite(void) {
		return bIsWrite;
	}
	int GetLBA(void) {
		return LBA;
	}
	char* GetData(void) {
		return input_data;
	}
private:
	bool bIsWrite = false;
	int LBA = 0;
	char input_data[20] = {};
	std::unique_ptr<FileManager> FileObj;
};
