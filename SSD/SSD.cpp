#include "gmock/gmock.h"
#include <fcntl.h>
#include <iostream>
using namespace std;
class ReadDriver {
public:
	string Read_SSD(int LBA, int nand_fd, int result_fd) {
		char* data;
		int bytes_read = 0;
		int fd;
		if (_lseek(nand_fd, LBA * 10, SEEK_SET) == -1) {
			//lseek error
		}
		bytes_read = _read(nand_fd, data, 10);
		if (bytes_read == -1) {
			//read error
		}
		_write(result_fd, data, 10);
	}
};
class WriteDriver {
public:
	string Write_SSD(int LBA, int write_fd, char* data) {
		int bytes_write = 0;
		if (_lseek(write_fd, LBA * 10, SEEK_SET) == -1) {
			//lseek error
		}
		bytes_write = _write(write_fd, data, 10);
		if (bytes_write == -1) {
			//write error
		}
	}
	int Format_SSD(int fd, char* buf) {
		//file doesn't exist - format
		int bytes_write = 0;
		if (fd == -1) {
			//open error
			return 0;
		}
		for (off_t offset = 0; offset < 100 * 10; offset += 10) {
			if (_lseek(fd, offset, SEEK_SET) == -1) {
				//lseek error
			}
			bytes_write = _write(fd, buf, 10);
			if (bytes_write == -1) {
				//write error
			}
		}
		//end format
	}
};
class SSDInterface {
public:
	int main(int argc, char* argv[]) {
		char ReadOrWrite = 0;
		int LBA = 0;
		char* data = (char*)"ABABABAB";
		int nand_fd;
		int result_fd;
		int bytes_write;
		int bytes_read;
		//check param
		if (argc == 3)
		{
			ReadOrWrite = (char)argv[1];
			LBA = strtol(argv[2], NULL, 16);
			data = argv[3];
		}
		else {
			cout << "Check argument count\n";
			return 0;
		}
		WriteDriver Writer;
		ReadDriver Reader;
		if ((ReadOrWrite == 'W') || (ReadOrWrite == 'w'))
		{
			if (_access(filename_w, O_RDWR) == -1) {
				_sopen_s(&nand_fd, filename_w, _O_CREAT | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
				Writer.Format_SSD(nand_fd, buf);
				Writer.Write_SSD(LBA, nand_fd, data);
			}
			else {
				_sopen_s(&nand_fd, filename_w, _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
				Writer.Write_SSD(LBA, nand_fd, data);
			}
		}
		else if ((ReadOrWrite == 'R') || (ReadOrWrite == 'r')) {
			if (_access(filename_w, O_RDWR) == -1) {
				//file doesn't exist - format
				_sopen_s(&nand_fd, filename_w, _O_CREAT | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
				Writer.Format_SSD(nand_fd, buf);
			}
			else {
				_sopen_s(&nand_fd, filename_w, _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
				if (nand_fd == -1) {
					//open error
					return 0;
				}
			}
			_sopen_s(&result_fd, filename_r, _O_CREAT | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
			Reader.Read_SSD(LBA, nand_fd, result_fd);
			_close(result_fd);
		}
		else {
			cout << "R or W cmd only\n";
		}
		_close(nand_fd);
		return 0;
	}
private:
	char filename_w[100] = "ssd_nand.txt";
	char filename_r[100] = "ssd_output.txt";
	char* buf = (char*)"00000000";
};

