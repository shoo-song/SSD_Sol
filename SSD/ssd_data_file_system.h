#pragma once

#include <io.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "fstream"
using namespace std;

class DataFileSystem {
 public:
  bool writeFile(int LBA, string data);
  bool readFile(int LBA, bool bCached, char *data);
  string getReadDataFromOutput();
  void writeInvalidLog();

 protected:
  std::fstream Nand_file_;
  std::fstream Output_file_;
  int BYTE_PER_LBA = 10;

  char filename_nand[100] = "ssd_nand.txt";
  char filename_output[100] = "ssd_output.txt";

  bool nandFileOpen(void);
  bool openNandFileWithMode(std::ios::openmode mode);
  bool openOutputFile(std::ios::openmode mode);
  bool outputFileOpenForRead(void);
  bool outputFileOpenForWrite(void);
  void writeToNandFile(string data, bool bData);
  void writeToOutputFile(string data, bool bData);
  bool fileExists(const char *filename);
  bool formatNandFile();
  void closeFiles();
  void closeFile(std::fstream &file);
  bool loadFromNand(int LBA, char *out_buf);
  bool loadData(int LBA, bool bCached, char *cached_data, char *out_buf);
  void writeToNand(int LBA, const std::string &data);
};
