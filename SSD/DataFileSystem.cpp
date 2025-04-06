#pragma once
#include "iostream"
#include "fstream"
#include <io.h>
#include <iomanip>
#include <iostream>
#include "DataFileSystem.h"
using namespace std;

bool DataFileSystem::OpenOutputFile(std::ios::openmode mode) {
	Output_file_.open(filename_output, mode | std::ios::binary);
	return Output_file_.is_open();
}
bool DataFileSystem::OpenNandFileWithMode(std::ios::openmode mode) {
	Nand_file_.open(filename_nand, mode);
	return Nand_file_.is_open();
}

bool DataFileSystem::fileExists(const char* filename) {
	return _access(filename, 0) != -1;
}

bool DataFileSystem::formatNandFile() {
	OpenNandFileWithMode(std::ios::out | std::ios::binary);

	if (!Nand_file_.is_open()) {
		return false;
	}

	for (int lba = 0; lba < 100; ++lba) {
		Nand_file_.seekp(lba * BYTE_PER_LBA);
		WriteToFile("0x00000000", true);
	}

	Nand_file_.close();
	return true;
}
bool DataFileSystem::NandFileOpen(void) {
	if (!fileExists(filename_nand)) {
		if (!formatNandFile()) {
			return false;
		}
	}

	OpenNandFileWithMode(std::ios::in | std::ios::out | std::ios::binary);
	return Nand_file_.is_open();
}

bool DataFileSystem::OutputFileOpenForWrite(void) {
	return OpenOutputFile(std::ios::out);
}
bool DataFileSystem::OutputFileOpenForRead(void) {
	return OpenOutputFile(std::ios::in);
}
void DataFileSystem::writeToNand(int LBA, const std::string& data) {
	Nand_file_.seekp(LBA * BYTE_PER_LBA);
	Nand_file_ << std::setw(10) << data;
	Nand_file_.flush();
}
bool DataFileSystem::WriteFile(int LBA, string data) {
	if (!NandFileOpen()) {
		return false;
	}

	writeToNand(LBA, data);

	WriteToFile(data, true);
	CloseFiles();
	
	return true;
}

void DataFileSystem::WriteToFile(string data, bool bData) {
	if (bData)
		Output_file_ << std::setw(10) << data;
	else 
		Output_file_ << data;

	Output_file_.flush();
}

bool DataFileSystem::loadFromNand(int LBA, char* out_buf) {
	if (!NandFileOpen()) {
		return false;
	}

	Nand_file_.seekg(LBA * BYTE_PER_LBA);
	Nand_file_.read(out_buf, BYTE_PER_LBA);
	CloseFiles();
	return true;
}

bool DataFileSystem::loadData(int LBA, bool bCached, char* cached_data, char* out_buf) {
	if (bCached) {
		memcpy(out_buf, cached_data, BYTE_PER_LBA);
		return true;
	}
	return loadFromNand(LBA, out_buf);
}

bool DataFileSystem::ReadFile(int LBA, bool bCached, char* cached_data) {
	bool result = true;
	char data_buf[20] = {};

	if (!loadData(LBA, bCached, cached_data, data_buf)) {
		return false;
	}

	if (!OutputFileOpenForWrite()) {
		return false;
	}

	WriteToFile(std::string(data_buf), true);
	CloseFiles();
	return true;
}

string DataFileSystem::getReadDataFromOutput() {

	if (!OutputFileOpenForRead()) {
		return {};
	}

	char data_buf[20] = {};		
	Output_file_.read(data_buf, BYTE_PER_LBA);
	CloseFiles();
	
	return data_buf;
}

void DataFileSystem::writeInvalidLog() {
	if (OutputFileOpenForWrite()) {		
		WriteToFile("ERROR", false);
		CloseFiles();
	}
}
void DataFileSystem::closeFile(std::fstream& file) {
	if (file.is_open()) {
		file.close();
	}
}
void DataFileSystem::CloseFiles() {
	closeFile(Output_file_);
	closeFile(Nand_file_);
}