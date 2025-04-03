#include "ssddriver.h"
#include "config.h"
#include "ShellUtil.cpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <windows.h>  // Windows API 사용

bool SsdDriver::executeCmd(std::string cmdLine) {
    char path[MAX_PATH];  // 실행 파일 전체 경로를 저장할 버퍼
    GetModuleFileNameA(NULL, path, MAX_PATH);  // 현재 실행 파일의 전체 경로 가져오기

    std::string exePath = path;  // std::string으로 변환
    size_t pos = exePath.find_last_of("\\");  // 마지막 '\'의 위치 찾기
    std::string dirPath = exePath.substr(0, pos);  // 실행 파일이 있는 디렉터리 경로

    // 실행할 command.exe 경로 설정
    std::string newCmd = dirPath + "\\" + cmdLine;

    int result = system(newCmd.c_str());

    if (result) {
        return false;
    }
    return true;
}

uint32_t SsdDriver::readSSD(int LBA) {
    std::string cmd = "SSD.exe R " + ShellUtil::getUtilObj().toTwoDigitString(LBA);
    const char* cmdLine= cmd.c_str();

    executeCmd(cmdLine);

    uint32_t ret = 0xFFFFFFFF;

    std::ifstream file(config::FILE_PATH); 
    if (!file.is_open())
    {
        //throw
        return ret;
    }

    std::string outputData;
    std::getline(file, outputData);
    ret = ShellUtil::getUtilObj().convertHexStringForData(outputData);

    return ret;
}

void SsdDriver::writeSSD(int LBA, uint32_t data) {
    std::string cmd = "SSD.exe W " + ShellUtil::getUtilObj().toTwoDigitString(LBA) + " "
        + ShellUtil::getUtilObj().toHexFormat(data);
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
