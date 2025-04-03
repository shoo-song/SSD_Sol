#pragma once

#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class FILEIO {
	virtual bool open(const std::string& filename, bool IsFormat, bool IsOpenForWrite) = 0;
};
class FileInterface : public FILEIO {
public:
	void CheckAndDoFormat(void);
	bool open(const std::string& filename, bool IsFormat, bool IsOpenForWrite) override;
	bool write(int LBA, string data);
	string read(int LBA);
	string getReadDataFromOutput();
	void writeInvalidLog();
	void close();
protected:
	std::fstream Write_file_;
	std::fstream Read_file_;
	char filename_output[100] = "ssd_output.txt";
	char filename_nand[100] = "ssd_nand.txt";
	char data_buf[20] = {};
	int BYTE_PER_LBA = 10;
};
