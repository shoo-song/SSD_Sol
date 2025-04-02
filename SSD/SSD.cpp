#include "gmock/gmock.h"
#include "File_Interface.cpp"
#include <iostream>
using namespace std;

class InvalidInputException : public std::exception {};
class SSDCommand {
public:
	SSDCommand() : FileObj(std::make_unique<FileInterface>()) {
	}

	void WriteInvalidLog()	{
		FileObj->writeInvalidLog();
		//throw InvalidInputException();
	}

	bool checkInvalidity(const char& CMD, string LBAstring,  char* data) {
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
		return true;
	}

	void parseArg(char CMD, string LBAstring, char* data = NULL) {
		if (checkInvalidity(CMD, LBAstring, data) != true ) return;

		if ((CMD == 'W') || (CMD == 'w')) {
			bIsWrite = true;
		}
		else if ((CMD == 'R') || (CMD == 'r')) {
			bIsWrite = false;
		}
	}
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

class SSDReadDriver {
public:
	SSDReadDriver() : FileObj(std::make_unique<FileInterface>()) {
	}
	string DoRead(int LBA) {
		return FileObj->read(LBA);
	}
private:
	std::unique_ptr<FileInterface> FileObj;
};
class SSDWriteDriver {
public:
	SSDWriteDriver() : FileObj(std::make_unique<FileInterface>()) {
	}
	void DoWrite(const int& LBA, const string& data) {
		if (FileObj == nullptr) return;
		FileObj->write(LBA, data);		
	}
private:
	std::unique_ptr<FileInterface> FileObj;
};

#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	SSDCommand InputParam;
	if (argc == 3) {
		InputParam.ReadInput(*argv[1], argv[2]);
	}
	else if (argc == 4) {
		InputParam.ReadInput(*argv[1], argv[2], argv[3]);
	}
	else {
		cout << "Invalid argument count\n";
	}
	SSDReadDriver Reader;
	SSDWriteDriver Writer;
	if (InputParam.IsWrite() == true) {
		Writer.DoWrite(InputParam.GetLBA(), InputParam.GetData());
	}
	else {
		Reader.DoRead(InputParam.GetLBA());
	}
	return 0;
}
#endif