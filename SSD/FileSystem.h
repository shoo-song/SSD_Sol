#pragma once

#include <cstdlib>   // system()
#include <cstdio>    // remove()
#include <memory>    // std::unique_ptr
#include <sys/stat.h> // mkdir(), stat()
#include <string>


class FileSystem {
public:
    bool directoryExists(const std::string& path) {
        struct stat info;
        return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
    }
    void removeDirectory(const std::string& path) {
        std::string command = "rmdir /S /Q " + path;
        system(command.c_str());
    }
    void createDirectory() {
        if (!directoryExists(bufferDir)) {
            system(("mkdir " + bufferDir).c_str());
        }
    }
    bool fileExists(const std::string& filename) {
        struct stat info;
        return (stat(filename.c_str(), &info) == 0);
    }

    void createFile(bool isEmpty) {
        if (isEmpty) {
            for (int i = 0; i < 5; i++) {
                std::string cmd = bufferDir + "/" + std::to_string(i) + "_" + emptyFile;
                std::ofstream file(cmd);
            }
        }
    }
private:
    std::string bufferDir = "buffer";
    std::string emptyFile = "empty";
};
