#pragma once
#include "iostream"
#include <fstream>
#include <cstdlib>   // system()
#include <cstdio>    // remove()
#include <memory>    // std::unique_ptr
#include <sys/stat.h> // mkdir(), stat()
#include <string>
#include <vector>
#include <windows.h> 
using namespace std;

class CommandFileSystem {
public:  

    bool directoryExists(const std::string& path);
    void removeDirectory(const std::string& path);
    void createDirectory();
    bool fileExists(const std::string& filename);

    void createFiles(void);

    void updateFileName(string oldName, string newName);
    std::vector<string> getCmdList();

    std::vector<string> makeCmdList();
private:
    std::string bufferDir = "buffer";
    std::string emptyFile = "empty";
    std::vector<std::string> fileNames;
};
