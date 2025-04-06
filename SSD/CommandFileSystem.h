#pragma once
#include <sys/stat.h>  // mkdir(), stat()
#include <windows.h>

#include <cstdio>   // remove()
#include <cstdlib>  // system()
#include <fstream>
#include <memory>  // std::unique_ptr
#include <string>
#include <vector>

#include "iostream"
using namespace std;

class CommandFileSystem {
 public:
  ~CommandFileSystem() = default;
  bool directoryExists(const std::string &path);
  void removeDirectory(const std::string &path);
  void createDirectory();
  bool fileExists(const std::string &filename);

  void createFiles(void);

  void updateFileName(std::string oldName, std::string newName);
  std::vector<string> getCmdList();
  std::vector<string> makeCmdList();

 private:
  std::string bufferDir = "buffer";
  std::string emptyFile = "empty";
  std::vector<std::string> fileNames;
};
