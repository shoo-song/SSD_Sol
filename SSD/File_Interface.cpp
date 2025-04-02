#include "iostream"
#include "fstream"
#include <iomanip>
#include <iostream>
using namespace std;
class FILEIO {
	virtual bool open(const std::string& filename, bool IsFormat, bool IsOpenForWrite) = 0;
};
class FileInterface : public FILEIO {
public:
	bool open(const std::string& filename, bool IsFormat, bool IsOpenForWrite) override {
		if (IsFormat == true) {
			Write_file_.open(filename, std::ios::out | std::ios::binary);
			return Write_file_.is_open();
		}
		else {
			if (IsOpenForWrite == true) {
				Write_file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
				return Write_file_.is_open();
			}
			else {
				Read_file_.open(filename, std::ios::in | std::ios::binary);
				return Read_file_.is_open();
			}
		}
	}

	bool write(char* data, int offset) {
		bool result = false;
		if (Write_file_.is_open()) {
			Write_file_.seekp(offset); 
			cout << data;
			Write_file_ << std::setw(10) <<  data;
			Write_file_.flush();
			result = true;
		}
		return result;
	}
	bool read(char* buf, int offset) {
		bool result = false;
		if (Read_file_.is_open()) {
			Read_file_.seekg(offset);
			Read_file_.read(buf, 10);
			result = true;
		}
		return result;
	}
	void close() {
		if (Write_file_.is_open()) {
			Write_file_.close();
		}
		if (Read_file_.is_open()) {
			Read_file_.close();
		}
	}
protected:
	std::fstream Write_file_;
	std::fstream Read_file_;
};