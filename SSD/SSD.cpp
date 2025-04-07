#pragma once
#include <iostream>
#include <vector>

#include "ssd_data_file_system.h"
using namespace std;

class SSD {
 public:
  SSD() : DataFileMgr(std::make_unique<DataFileSystem>()) {}
  void doRead(int LBA) { DataFileMgr->readFile(LBA, false, nullptr); }

  void doCacheRead(int LBA, char *data) {
    DataFileMgr->readFile(LBA, true, data);
  }
  void doWrite(const int &LBA, const string &data) {
    DataFileMgr->writeFile(LBA, data);
  }
  void doErase(const int &StartLBA, int LBACount) {
    char ErasePattern[20] = "0x00000000";
    for (int offset = 0; offset < LBACount; offset++) {
      DataFileMgr->writeFile(StartLBA + offset, ErasePattern);
    }
  }

 private:
  std::unique_ptr<DataFileSystem> DataFileMgr;
};
