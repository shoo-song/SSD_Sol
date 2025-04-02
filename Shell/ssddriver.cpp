#include "ssddriver.h"
#include "config.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

bool SsdDriver::executeCmd(std::string cmdLine) {
    int result = system(cmdLine.c_str());

    if (result) {
        return true;
    }
    return false;
}

uint32_t SsdDriver::readSSD(int LBA) {

    std::string cmd = "SSD.exe R " + std::to_string(LBA);
    const char* cmdLine= cmd.c_str();

    executeCmd(cmdLine);

    uint32_t ret = 0xFFFFFFFF;

    std::ifstream file(config::FILE_PATH); 
    if (!file.is_open()) return ret;

    std::string line;
    int current = 1;
    while (std::getline(file, line)) {
        if (current == LBA) return ConvertStrToUint32(line);
        ++current;
    }

    return ret;
}

void SsdDriver::writeSSD(int LBA, uint32_t data) {
    std::string cmd = "SSD.exe W " + std::to_string(LBA) + ' ' + std::to_string(data);
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);
}


std::string SsdDriver::TrimData(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

uint32_t SsdDriver::ConvertStrToUint32(const std::string& hexStr) {
    std::string str = TrimData(hexStr);

    // 0x 또는 0X 제거
    if (str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X") {
        str = str.substr(2);
    }

    uint32_t value = 0;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> value;
    return value;
}
