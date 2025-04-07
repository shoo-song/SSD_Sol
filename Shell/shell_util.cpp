#include "shell_util.h"

ShellUtil& ShellUtil::getUtilObj() {
    static ShellUtil shellInterfaceUtil;
    return shellInterfaceUtil;
}

// LBA 문자열 변환 (10진수, 0~99)
unsigned int ShellUtil::convertDecimalStringForLba(const std::string& input) {
    size_t pos;
    unsigned int value = std::stoi(input, &pos, 10);

    // 변환된 길이 확인 (예: "12abc" 방지)
    if (pos != input.length()) {
        throw ShellException("Invalid characters in input: " + input);
    }

    // 범위 검사
    if (value < 0 || value > 99) {
        throw ShellException("Value out of range (0-99): " + input);
    }

    return value;
}

// Size 문자열 변환 (10진수, 0~100)
unsigned int ShellUtil::convertStrForSize(const std::string& LBA1, const std::string& size) {
    size_t startPos, sizePos;
    unsigned int startLBA = std::stoi(LBA1, &startPos, 10);
    unsigned int sizeLBA = std::stoi(size, &sizePos, 10);
    unsigned int totalSize = startLBA + sizeLBA;

    // 변환된 길이 확인 (예: "12abc" 방지)
    if (sizePos != size.length()) {
        throw ShellException("Invalid characters in input: " + size);
    }

    // 범위 검사
    if (sizeLBA < 0 || sizeLBA > 100) {
        throw ShellException("Value out of range (0-100): " + size);
    }

    // LBA + Range가 MAX 초과
    if (totalSize > 100) {
        throw ShellException("Erase Range over Max " + size);
    }

    return sizeLBA;
}

unsigned int ShellUtil::convertLBAtoSize(const std::string& LBA1, const std::string& LBA2) {
    size_t pos;
    unsigned int startLBA = std::stoi(LBA1, &pos, 10);
    unsigned int endLBA = std::stoi(LBA2, &pos, 10);

    // 변환된 길이 확인 (예: "12abc" 방지)
    if (pos != LBA2.length()) {
        throw ShellException("Invalid characters in input: " + LBA2);
    }

    // 범위 검사
    if (endLBA < 0 || endLBA > 99) {
        throw ShellException("Value out of range (0-99): " + LBA2);
    }

    if (startLBA > endLBA) {
        throw ShellException("Start LBA : " + LBA1 + ' ' + "End LBA : " + LBA2);
    }

    unsigned int size = endLBA - startLBA + 1;

    if (startLBA + size > 100) {
        throw ShellException("Erase Range over Max " + size);
    }

    return size;
}

// Data 문자열 변환 (16진수, "0x" + 8자리)
unsigned int ShellUtil::convertHexStringForData(const std::string& input) {
    // 길이 체크
    if (input.length() != 10 || input.substr(0, 2) != "0x") {
        throw ShellException("Invalid hex format: " + input);
    }

    size_t pos;
    unsigned int value = std::stoul(input, &pos, 16);

    // 변환된 길이 확인 (예: "0x1234567G" 방지)
    if (pos != input.length()) {
        throw ShellException("Invalid characters in input: " + input);
    }

    return value;
}

vector<std::string> ShellUtil::splitString(const std::string& s) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string word;

    while (iss >> word) {  // 공백과 탭을 자동으로 구분
        result.push_back(word);
    }

    return result;
}

string ShellUtil::toTwoDigitString(unsigned int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

string ShellUtil::toHexFormat(unsigned int value) {
    std::ostringstream oss;
    oss << "0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value;
    return oss.str();
}

ShellUtil::ShellUtil() {}