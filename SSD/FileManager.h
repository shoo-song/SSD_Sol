#pragma once

#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;


class FileManager {
public:
	bool NandFileOpen(void);
	bool OutputFileOpen(void);
	bool WriteFile(int LBA, string data);
	bool ReadFile(int LBA);
	string getReadDataFromOutput();
	void writeInvalidLog();
	void CloseFiles();
protected:
	std::fstream Nand_file_;
	std::fstream Output_file_;
	int BYTE_PER_LBA = 10;
};
