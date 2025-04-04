#pragma once

#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include <string>
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

	char filename_nand[100] = "ssd_nand.txt";
	char filename_output[100] = "ssd_output.txt";

	bool OpenFile(std::ios::openmode mode);
	void WriteToFile(string data, bool bData);

};
