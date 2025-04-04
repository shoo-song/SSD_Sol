#pragma once
#include "iostream"
#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include "DataFileSystem.h"
using namespace std;

bool DataFileSystem::NandFileOpen(void) {
	char filename_nand[100] = "ssd_nand.txt";
	if (_access(filename_nand, 0) == -1) {
		Nand_file_.open(filename_nand, std::ios::out | std::ios::binary);
		if (Nand_file_.is_open()) {
			for (int formatLBA = 0; formatLBA < 100; formatLBA++) {
				Nand_file_.seekp(formatLBA * BYTE_PER_LBA);
				Nand_file_ << std::setw(10) << "0x00000000";
				Nand_file_.flush();
			}
			Nand_file_.close();
		}
		else {
			return false;
		}
	}
	Nand_file_.open(filename_nand, std::ios::in | std::ios::out | std::ios::binary);
	return Nand_file_.is_open();
}
bool DataFileSystem::OutputFileOpenForWrite(void) {
	char filename_output[100] = "ssd_output.txt";
	Output_file_.open(filename_output, std::ios::out | std::ios::binary);
	return Output_file_.is_open();
}
bool DataFileSystem::OutputFileOpenForRead(void) {
	char filename_output[100] = "ssd_output.txt";
	Output_file_.open(filename_output, std::ios::in | std::ios::binary);
	return Output_file_.is_open();
}

bool DataFileSystem::WriteFile(int LBA, string data) {
	if (NandFileOpen()) {
		Nand_file_.seekp(LBA * BYTE_PER_LBA);
		Nand_file_ << std::setw(10) << data;
		Nand_file_.flush();
		CloseFiles();
	}
	else {
		return false;
	}
	return true;
}
bool DataFileSystem::ReadFile(int LBA, bool bCached, char* cached_data) {
	bool result = true;
	char data_buf[20] = {};
	if (bCached != true) {
		if (NandFileOpen()) {
			Nand_file_.seekg(LBA * BYTE_PER_LBA);
			Nand_file_.read(data_buf, 10);
		}
		else {
			return false;
		}
		CloseFiles();
	}
	else {
		memcpy(data_buf, cached_data, BYTE_PER_LBA);
	}

	if (OutputFileOpenForWrite()) {
		Output_file_ << std::setw(10) << data_buf;
		Output_file_.flush();
	}
	else {
		return false;
	}
	CloseFiles();
	return true;
}

string DataFileSystem::getReadDataFromOutput() {
	char data_buf[20] = {};
	if (OutputFileOpenForRead()) {
		Output_file_.read(data_buf, 10);
		CloseFiles();
	}
	return data_buf;
}

void DataFileSystem::writeInvalidLog() {
	if (OutputFileOpenForWrite()) {
		Output_file_ << "ERROR";
		Output_file_.flush();
		CloseFiles();
	}
}

void DataFileSystem::CloseFiles() {
	if (Output_file_.is_open()) {
		Output_file_.close();
	}
	if (Nand_file_.is_open()) {
		Nand_file_.close();
	}
}