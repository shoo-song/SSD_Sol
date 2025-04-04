#include "logger.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "logger_exception.cpp"

namespace fs = std::filesystem;

// �̱��� �ν��Ͻ�
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// ���� ���� ��ü ����
Logger& logger = Logger::getInstance();

Logger::~Logger() { closeLogFile(); }

void Logger::setLogFile(const std::string& filename) {
    try {
        std::filesystem::create_directories(logDirectory_);
        std::string actualFileName = filename;
        if (actualFileName.find('.') == std::string::npos) {
            actualFileName += ".log";
        }
    } catch (const std::exception& e) {
        throw LoggerException("�α� ���丮 ���� ����: " + std::string(e.what()));
    }

    logFileName_ = logDirectory_ + "/" + filename;
    openLogFile();
}

void Logger::openLogFile() {
    closeLogFile();
    logFile_.open(logFileName_, std::ios::app);
    if (!logFile_) {
        throw LoggerException("�α� ���� ���� ����: " + logFileName_);
    }
}

void Logger::closeLogFile() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

std::string Logger::getTimestampedFilename() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_c);

    std::ostringstream oss;
    oss << logDirectory_ << "/until_" << std::put_time(&now_tm, "%y%m%d_%Hh_%Mm_%Ss") << ".log";
    return oss.str();
}

void Logger::rotateIfNeeded() {
    if (!fs::exists(logFileName_))
        return;

    constexpr std::uintmax_t MAX_SIZE = 10 * 1024;  // 10KB
    std::uintmax_t size = fs::file_size(logFileName_);
    if (size < MAX_SIZE)
        return;

    // 1. �ݰ� �� �̸� ���� �� �ٽ� ����
    closeLogFile();

    std::string backupName = getTimestampedFilename();
    try {
        fs::rename(logFileName_, backupName);
    } catch (const fs::filesystem_error& e) {
        throw LoggerException("�α� ���� �̸� ���� ����: " + std::string(e.what()));
    }

    openLogFile();

    // 2. ��� �αװ� 2�� �̻��̸� ���� ������ ������ .zip Ȯ���ڷ� ����
    try {
        std::vector<fs::directory_entry> backups;

        for (const auto& entry : fs::directory_iterator(logDirectory_)) {
            if (entry.is_regular_file()) {
                const std::string& name = entry.path().filename().string();
                if (name.rfind("until_", 0) == 0 && entry.path().extension() == ".log") {
                    backups.push_back(entry);
                }
            }
        }

        if (backups.size() >= 2) {
            std::sort(backups.begin(), backups.end(), [](const auto& a, const auto& b) {
                return fs::last_write_time(a) < fs::last_write_time(b);
            });

            fs::path oldest = backups.front().path();
            fs::path zipped = oldest;
            zipped.replace_extension(".zip");

            try {
                fs::rename(oldest, zipped);
            } catch (const std::exception& e) {
                throw LoggerException("������ �α� ���� ���� �̸� ���� ����: " +
                                      std::string(e.what()));
            }
        }
    } catch (const std::exception& e) {
        throw LoggerException("��� �α� ���� ó�� �� ����: " + std::string(e.what()));
    }
}

void Logger::print(const std::string& location, const std::string& message) {
    try {
        rotateIfNeeded();

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm;
        localtime_s(&now_tm, &now_c);

        std::ostringstream oss;
        oss << std::put_time(&now_tm, "[%y.%m.%d %H:%M] ") << std::left << std::setw(30) << location
            << " : " << message;

        std::string output = oss.str();
        // std::cout << output << std::endl;

        if (logFile_.is_open()) {
            logFile_ << output << std::endl;
        } else {
            throw LoggerException("�α� ������ ���� ���� �ʽ��ϴ�.");
        }
    } catch (const std::exception& e) {
        throw LoggerException("print ����: " + std::string(e.what()));
    }
}
