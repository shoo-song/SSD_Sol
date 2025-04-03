#pragma once
#include <iostream>
#include "FileManager.h"
using namespace std;
class SSD {
public:
	SSD() : FileMgr(std::make_unique<FileManager>()) {
	}
	void DoRead(int LBA) {
		if (FileMgr == nullptr) return;
		FileMgr->ReadFile(LBA);
	}
	void DoWrite(const int& LBA, const string& data) {
		if (FileMgr == nullptr) return;
		FileMgr->WriteFile(LBA, data);
	}
	void DoErase(const int& StartLBA, int LBACount) {
		if (FileMgr == nullptr) return;
		char ErasePattern[20] = "0x00000000";
		for (int offset = 0; offset < LBACount; offset++) {
			FileMgr->WriteFile(StartLBA + offset, ErasePattern);
		}
	}
private:
	std::unique_ptr<FileManager> FileMgr;
};

