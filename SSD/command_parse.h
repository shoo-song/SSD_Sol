#pragma once
#include "FIle_interface.h"

class SSDCommand {
public:
	SSDCommand() : FileObj(std::make_unique<FileInterface>()) {
	}

	void WriteInvalidLog();

	bool checkInvalidity(const char& CMD, string LBAstring, char* data);

	void parseArg(char CMD, string LBAstring, char* data = NULL);

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
	std::unique_ptr<FileInterface> FileObj;
};
