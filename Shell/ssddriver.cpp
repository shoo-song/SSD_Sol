#include "ssddriver.h"
#include "config.h"
#include "ShellUtil.cpp"
#include "ssddriver_exception.cpp"
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
        try {
            return false;
        }
        catch (exception e) {
            throw e;
        }
        catch (SsdExecuteException e) {
            throw SsdExecuteException();
        }
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
        try {
            return ret;
        }
        catch (exception e) {
            throw e;
        }
        catch (FileNotFoundException e) {
            throw FileNotFoundException();
        }
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
