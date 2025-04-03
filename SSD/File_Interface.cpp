#pragma once
#include "iostream"
#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include "File_Interface.h"
using namespace std;

void FileInterface::CheckAndDoFormat(void) {
	if (_access(filename_nand, 0) == -1) {
		open(filename_nand, true, true);
		if (Write_file_.is_open()) {
			for (int formatLBA = 0; formatLBA < 100; formatLBA++) {
				Write_file_.seekp(formatLBA * BYTE_PER_LBA);
				Write_file_ << std::setw(10) << "0x00000000";
				Write_file_.flush();
			}
		}
		Write_file_.close();
	}
}

bool FileInterface::open(const std::string& filename, bool IsFormat, bool IsOpenForWrite) {
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
			Read_file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
			return Read_file_.is_open();
		}
	}
}

bool FileInterface::write(int LBA, string data) {
	bool result = true;
	CheckAndDoFormat();
	open(filename_nand, false, true);
	if (Write_file_.is_open()) {
		Write_file_.seekp(LBA * BYTE_PER_LBA);
		Write_file_ << std::setw(10) << data;
		Write_file_.flush();
	}
	close();
	return result;
}

string FileInterface::read(int LBA) {
	bool result = true;
	CheckAndDoFormat();

	open(filename_nand, false, false);
	if (Read_file_.is_open()) {
		Read_file_.seekg(LBA * BYTE_PER_LBA);
		Read_file_.read(data_buf, 10);
	}

	open(filename_output, true, true);
	if (Write_file_.is_open()) {
		Write_file_ << std::setw(10) << data_buf;
		Write_file_.flush();
	}
	close();

	return data_buf;
}

string FileInterface::getReadDataFromOutput() {
	open(filename_output, false, false);
	if (Read_file_.is_open()) {
		Read_file_.read(data_buf, 10);
	}
	return data_buf;
}

void FileInterface::writeInvalidLog() {
	open(filename_output, true, false);
	cout << "teststat " << Write_file_.is_open();
	if (Write_file_.is_open()) {
		Write_file_ << "ERROR";
		Write_file_.flush();
	}
	close();
}

void FileInterface::close() {
	if (Write_file_.is_open()) {
		Write_file_.close();
	}
	if (Read_file_.is_open()) {
		Read_file_.close();
	}
}