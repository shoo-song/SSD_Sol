#include "gmock/gmock.h"
#include <fcntl.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
	char ReadOrWrite = 0;
	int LBA = 0;
	char* data = (char*)"ABABABAB";
	char filename[100] = "ssd_nand.txt";
	int fd;
	int bytes_write;
	char* buf = (char*)"00000000";
	//check param
	if (argc == 3)
	{
		ReadOrWrite = (char)argv[1];
		LBA = strtol(argv[2], NULL, 16);
		data = argv[3];
	}
	else{
		cout << "Check argument count\n";
		return 0;
	}
	if ((ReadOrWrite == 'W') || (ReadOrWrite == 'w'))
	{
		if (_access(filename, O_RDWR) == -1) {
			//file doesn't exist - format
			_sopen_s(&fd, filename, _O_CREAT | _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
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

			if (_lseek(fd, LBA * 10, SEEK_SET) == -1) {
				//lseek error
			}
			int bytes_write = _write(fd, data, 10);
			if (bytes_write == -1) {
				//write error
			}
		}
		else {
			_sopen_s(&fd, filename, _O_RDWR, _SH_DENYNO, _S_IREAD | _S_IWRITE);
			if (fd == -1) {
				//open error
				return 0;
			}
			if (_lseek(fd, LBA, SEEK_SET) == -1) {
				//lseek error
			}
			int bytes_write = _write(fd, data, 10);
			if (bytes_write == -1) {
				//write error
			}
		}
	}
	else {
		cout << "R or W cmd only\n";
	}
	_close(fd);
	return 0;
	//::testing::InitGoogleMock();
	//return RUN_ALL_TESTS();
}