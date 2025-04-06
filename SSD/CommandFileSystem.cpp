#include "CommandFileSystem.h"

bool CommandFileSystem::directoryExists(const std::string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

void CommandFileSystem::removeDirectory(const std::string& path) {
    std::string command = "rmdir /S /Q " + path;
    system(command.c_str());
}

void CommandFileSystem::createDirectory() {
    if (!directoryExists(bufferDir)) {
        system(("mkdir " + bufferDir).c_str());
    }
}

bool CommandFileSystem::fileExists(const std::string& filename) {
    struct stat info;
    string name = bufferDir + "/" + filename;
    return (stat(name.c_str(), &info) == 0);    // 존재하면 0
}

void CommandFileSystem::createFiles(void) {
    for (int i = 0; i < 5; i++) {
        std::string name = "/" + std::to_string(i) + "_" + emptyFile;
        std::string cmd = bufferDir + name;
        std::ofstream file(cmd);
    }
}

void CommandFileSystem::updateFileName(string oldName, string newName) {
    string oldName_ = bufferDir + "/" + oldName;
    string newName_ = bufferDir + "/" + newName;
    if (!fileExists(oldName)) {
        std::cout << "파일이 존재하지 않습니다: " << oldName << std::endl;    // errro log 필요
    }
    
    if (rename(oldName_.c_str(), newName_.c_str()) != 0) {        
        std::cout << "[ERROR] 파일 이름 변경 실패 " << std::endl;
    }
}

std::vector<string> CommandFileSystem::getCmdList() {
    // check and create dir
    if (!directoryExists("buffer")) {
        createDirectory();
        createFiles();
    }
    return  makeCmdList();
}

std::vector<string> CommandFileSystem::makeCmdList() {
    fileNames.clear();
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((bufferDir + "\\*").c_str(), &findFileData);
    do {
        std::string fileName = findFileData.cFileName;

        // 첫 글자가 0~4인지 확인
        if (fileName[0] >= '0' && fileName[0] <= '4' && fileName[1] == '_') {
            fileNames.push_back(fileName);
        }

    } while (FindNextFileA(hFind, &findFileData) != 0);
    return fileNames;
}

