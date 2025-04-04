#pragma once
#include <iostream>
#include <vector>
#include "DataFileSystem.h"
using namespace std;

class SSD {
public:
	SSD() : DataFileMgr(std::make_unique<DataFileSystem>())
	{
	}
	void DoRead(int LBA) {
		DataFileMgr->ReadFile(LBA, false, nullptr);
	}

	void DoCachedRead(int LBA, char* data) {
		DataFileMgr->ReadFile(LBA, true, data);
	}
	void DoWrite(const int& LBA, const string& data) {

		DataFileMgr->WriteFile(LBA, data);
	}
	void DoErase(const int& StartLBA, int LBACount) {

		char ErasePattern[20] = "0x00000000";
		for (int offset = 0; offset < LBACount; offset++) {
			DataFileMgr->WriteFile(StartLBA + offset, ErasePattern);
		}
	}
private:
	std::unique_ptr<DataFileSystem> DataFileMgr;
};

