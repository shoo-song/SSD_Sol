#pragma once
#include <iostream>
#include "File_Interface.h"
using namespace std;

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
