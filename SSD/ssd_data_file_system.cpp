#pragma once
#include "ssd_data_file_system.h"

#include <io.h>

#include <iomanip>
#include <iostream>

#include "fstream"
#include "iostream"
using namespace std;

bool DataFileSystem::openOutputFile(std::ios::openmode mode) {
  Output_file_.open(filename_output, mode | std::ios::binary);
  return Output_file_.is_open();
}
bool DataFileSystem::openNandFileWithMode(std::ios::openmode mode) {
  Nand_file_.open(filename_nand, mode);
  return Nand_file_.is_open();
}

bool DataFileSystem::fileExists(const char *filename) {
  return _access(filename, 0) != -1;
}

bool DataFileSystem::formatNandFile() {
  openNandFileWithMode(std::ios::out | std::ios::binary);

  if (!Nand_file_.is_open()) {
    return false;
  }
  for (int lba = 0; lba < 100; lba++) {
    Nand_file_.seekp(lba * BYTE_PER_LBA);
    writeToNandFile("0x00000000", true);
  }
  Nand_file_.close();

  return true;
}
bool DataFileSystem::nandFileOpen(void) {
  if (!fileExists(filename_nand)) {
    if (!formatNandFile()) {
      return false;
    }
  }

  openNandFileWithMode(std::ios::in | std::ios::out | std::ios::binary);
  return Nand_file_.is_open();
}

bool DataFileSystem::outputFileOpenForWrite(void) {
  return openOutputFile(std::ios::out);
}
bool DataFileSystem::outputFileOpenForRead(void) {
  return openOutputFile(std::ios::in);
}
void DataFileSystem::writeToNand(int LBA, const std::string &data) {
  Nand_file_.seekp(LBA * BYTE_PER_LBA);
  Nand_file_ << std::setw(10) << data;
  Nand_file_.flush();
}
bool DataFileSystem::writeFile(int LBA, string data) {
  if (!nandFileOpen()) {
    return false;
  }

  writeToNand(LBA, data);

  writeToNandFile(data, true);
  closeFiles();

  return true;
}
void DataFileSystem::writeToNandFile(string data, bool bData) {
    if (bData)
        Nand_file_ << std::setw(10) << data;
    else
        Nand_file_ << data;

    Nand_file_.flush();
}
void DataFileSystem::writeToOutputFile(string data, bool bData) {
  if (bData)
    Output_file_ << std::setw(10) << data;
  else
    Output_file_ << data;

  Output_file_.flush();
}

bool DataFileSystem::loadFromNand(int LBA, char *out_buf) {
  if (!nandFileOpen()) {
    return false;
  }

  Nand_file_.seekg(LBA * BYTE_PER_LBA);
  Nand_file_.read(out_buf, BYTE_PER_LBA);
  closeFiles();
  return true;
}

bool DataFileSystem::loadData(int LBA, bool bCached, char *cached_data,
                              char *out_buf) {
  if (bCached) {
    memcpy(out_buf, cached_data, BYTE_PER_LBA);
    return true;
  }
  return loadFromNand(LBA, out_buf);
}

bool DataFileSystem::readFile(int LBA, bool bCached, char *cached_data) {
  bool result = true;
  char data_buf[20] = {};

  if (!loadData(LBA, bCached, cached_data, data_buf)) {
    return false;
  }

  if (!outputFileOpenForWrite()) {
    return false;
  }

  writeToOutputFile(std::string(data_buf), true);
  closeFiles();
  return true;
}

string DataFileSystem::getReadDataFromOutput() {
  if (!outputFileOpenForRead()) {
    return {};
  }

  char data_buf[20] = {};
  Output_file_.read(data_buf, BYTE_PER_LBA);
  closeFiles();

  return data_buf;
}

void DataFileSystem::writeInvalidLog() {
  if (outputFileOpenForWrite()) {
    writeToOutputFile("ERROR", false);
    closeFiles();
  }
}
void DataFileSystem::closeFile(std::fstream &file) {
  if (file.is_open()) {
    file.close();
  }
}
void DataFileSystem::closeFiles() {
  closeFile(Output_file_);
  closeFile(Nand_file_);
}