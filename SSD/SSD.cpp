#include "gmock/gmock.h"
#include "File_Interface.cpp"
#include <iostream>
using namespace std;
class InputData {
public:
	void ReadInput(char RorW, string LBAstring, char* data = NULL) {
		if ((RorW == 'W') || (RorW == 'w')) {
			bIsWrite = true;
		}
		else if ((RorW == 'R') || (RorW == 'r')) {
			bIsWrite = false;
		}
		else {
			cout << "R or W needed\n";
		}
		LBA = stoi(LBAstring);
		if (LBA >= 100) {
			cout << "LBA overflow\n";
		}
		if (data != NULL) {
			strcpy_s(input_data, data);
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
};
class SSDReadDriver {
public:
	SSDReadDriver(FileInterface* file) {
		FileManager = file;
	}
	void DoRead(int LBA) {
		FileManager->read(LBA);
	}
private:
	FileInterface* FileManager;
};
class SSDWriteDriver {
public:
	SSDWriteDriver(FileInterface* file) {
		FileManager = file;
	}
	void DoWrite(int LBA, char* data) {
		FileManager->write(LBA, data);
	}
private:
	FileInterface* FileManager;
};

#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	InputData InputParam;
	if (argc == 3) {
		InputParam.ReadInput(*argv[1], argv[2]);
	}
	else if (argc == 4) {
		InputParam.ReadInput(*argv[1], argv[2], argv[3]);
	}
	else {
		cout << "Invalid argument count\n";
	}
	FileInterface FileManager;
	SSDReadDriver Reader{ &FileManager };
	SSDWriteDriver Writer{ &FileManager };
	if (InputParam.IsWrite() == true) {
		Writer.DoWrite(InputParam.GetLBA(), InputParam.GetData());
	}
	else {
		Reader.DoRead(InputParam.GetLBA());
	}
	return 0;
}
#endif