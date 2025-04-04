#include "ssddriver.h"

#include <windows.h>  // Windows API ���

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "ShellUtil.cpp"
#include "config.h"
#include "logger.h"
#include "ssddriver_exception.cpp"

bool SsdDriver::executeCmd(std::string cmdLine) {
    char path[MAX_PATH];                       // ���� ���� ��ü ��θ� ������ ����
    GetModuleFileNameA(NULL, path, MAX_PATH);  // ���� ���� ������ ��ü ��� ��������

    logger.print("SsdDriver.executeCmd()", "execute " + cmdLine);

    std::string exePath = path;                    // std::string���� ��ȯ
    size_t pos = exePath.find_last_of("\\");       // ������ '\'�� ��ġ ã��
    std::string dirPath = exePath.substr(0, pos);  // ���� ������ �ִ� ���͸� ���

    // ������ command.exe ��� ����
    std::string newCmd = dirPath + "\\" + cmdLine;

    int result = system(newCmd.c_str());

    if (result) {
        throw SsdExecuteException();
    }
    return true;
}

uint32_t SsdDriver::readSSD(int LBA) {
    std::string cmd = config::READ_MARKER + ShellUtil::getUtilObj().toTwoDigitString(LBA);
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);

    uint32_t ret = 0xFFFFFFFF;

    std::ifstream file(config::FILE_PATH);

    if (!file.is_open()) {
        throw FileNotFoundException();
    }

    std::string outputData;
    std::getline(file, outputData);
    ret = ShellUtil::getUtilObj().convertHexStringForData(outputData);

    return ret;
}

void SsdDriver::writeSSD(int LBA, uint32_t data) {
    std::string cmd = config::WRITE_MARKER + ShellUtil::getUtilObj().toTwoDigitString(LBA) + " " +
                      ShellUtil::getUtilObj().toHexFormat(data);
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);
}

void SsdDriver::eraseSSD(int LBA, int size) {
    std::string cmd = config::ERASE_MARKER + ShellUtil::getUtilObj().toTwoDigitString(LBA) + " " +
                      std::to_string(size);
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);
}

void SsdDriver::flushSSD() {
    std::string cmd = config::FLUSH_MARKER;
    const char* cmdLine = cmd.c_str();

    executeCmd(cmdLine);
}