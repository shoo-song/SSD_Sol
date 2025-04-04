#pragma once

#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include <string>
//#include "FIleSystem.h"
using namespace std;


class DataFileSystem {
public:
	bool NandFileOpen(void);
	bool OutputFileOpenForRead(void);
	bool OutputFileOpenForWrite(void);
	bool WriteFile(int LBA, string data);
	bool ReadFile(int LBA, bool bCached, char* data);
	string getReadDataFromOutput();
	void writeInvalidLog();
	void CloseFiles();

protected:
	std::fstream Nand_file_;
	std::fstream Output_file_;
	int BYTE_PER_LBA = 10;
};