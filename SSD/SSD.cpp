#include "gmock/gmock.h"
#include "File_Interface.cpp"
#include <fcntl.h>
#include <iostream>
using namespace std;
#define BYTE_PER_LBA (10)
class ReadDriver {
public:
};
class WriteDriver {
public:
};

#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	char filename_nand[100] = "ssd_nand.txt";
	char filename_output[100] = "ssd_output.txt";
	char read_buf[20] = "00000000";
	char write_buf[20] = "00000000";
	char ReadOrWrite = 0;
	int LBA = 0;
	int nand_fd;
	int result_fd;
	int bytes_write;
	int bytes_read;
	//check param
	ReadOrWrite = *argv[1];
	LBA = stoi(argv[2]);
	if ((ReadOrWrite == 'W') || (ReadOrWrite == 'w')) {
		strcpy_s(write_buf,argv[3]);
	}
	// parsing end
	FileInterface NandFileForWrite;
	FileInterface NandFileForRead;
	FileInterface OutputFileForWrite;
	bool bNeedFormat = false;
	if (_access(filename_nand, O_RDWR) == -1) {
		bNeedFormat = true;
		cout << "need format" << endl;
	}

	if (bNeedFormat == true) {
		NandFileForWrite.open(filename_nand, true/*IsFormat*/, true/*IsOpenForWrite*/);
		for (int formatLBA = 0; formatLBA < 100; formatLBA++) {
			NandFileForWrite.write(write_buf, formatLBA * BYTE_PER_LBA);
		}
		NandFileForWrite.close();
	}

	if ((ReadOrWrite == 'W') || (ReadOrWrite == 'w')) {
		NandFileForWrite.open(filename_nand, false, true/*IsOpenForWrite*/);
		NandFileForWrite.write(write_buf, LBA * BYTE_PER_LBA);
		NandFileForWrite.close();
	}
	else if ((ReadOrWrite == 'R') || (ReadOrWrite == 'r')) {
		NandFileForRead.open(filename_nand, false, false/*IsOpenForWrite*/);
		NandFileForRead.read(read_buf, LBA * BYTE_PER_LBA);
		NandFileForRead.close();

		OutputFileForWrite.open(filename_output, true, true/*IsOpenForWrite*/);
		OutputFileForWrite.write(read_buf, 0);
		OutputFileForWrite.close();
	}
	else {
		cout << "Read or Write needed" << endl;
	}
	
	return 0;
}
#endif