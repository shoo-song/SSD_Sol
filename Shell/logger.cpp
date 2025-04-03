#include "logger.h"
#include "logger_exception.cpp"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// 싱글턴 인스턴스
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// 전역 참조 객체 정의
Logger& logger = Logger::getInstance();

Logger::~Logger() {
    closeLogFile();
}

void Logger::setLogFile(const std::string& filename) {
    try {
        std::filesystem::create_directories(logDirectory_);
        std::string actualFileName = filename;
        if (actualFileName.find('.') == std::string::npos) {
            actualFileName += ".log";
        }
    }
    catch (const std::exception& e) {
        throw LoggerException("로그 디렉토리 생성 실패: " + std::string(e.what()));
    }

    logFileName_ = logDirectory_ + "/" + filename;
    openLogFile();
}

void Logger::openLogFile() {
    closeLogFile();
    logFile_.open(logFileName_, std::ios::app);
    if (!logFile_) {
        throw LoggerException("로그 파일 열기 실패: " + logFileName_);
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
    oss << logDirectory_ << "/until_"
        << std::put_time(&now_tm, "%y%m%d_%Hh_%Mm_%Ss")
        << ".log";
    return oss.str();
}

void Logger::rotateIfNeeded() {
    if (!fs::exists(logFileName_)) return;

    constexpr std::uintmax_t MAX_SIZE = 10 * 1024;  // 10KB
    std::uintmax_t size = fs::file_size(logFileName_);
    if (size < MAX_SIZE) return;

    // 1. 닫고 → 이름 변경 → 다시 열기
    closeLogFile();

    std::string backupName = getTimestampedFilename();
    try {
        fs::rename(logFileName_, backupName);
    }
    catch (const fs::filesystem_error& e) {
        throw LoggerException("로그 파일 이름 변경 실패: " + std::string(e.what()));
    }

    openLogFile();

    // 2. 백업 로그가 2개 이상이면 가장 오래된 파일을 .zip 확장자로 변경
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
            }
            catch (const std::exception& e) {
                throw LoggerException("오래된 로그 파일 압축 이름 변경 실패: " + std::string(e.what()));
            }
        }
    }
    catch (const std::exception& e) {
        throw LoggerException("백업 로그 파일 처리 중 오류: " + std::string(e.what()));
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
        oss << std::put_time(&now_tm, "[%y.%m.%d %H:%M] ")
            << std::left << std::setw(30) << location
            << " : " << message;

        std::string output = oss.str();
        std::cout << output << std::endl;

        if (logFile_.is_open()) {
            logFile_ << output << std::endl;
        }
        else {
            throw LoggerException("로그 파일이 열려 있지 않습니다.");
        }
    } catch (const std::exception& e) {
        throw LoggerException("print 실패: " + std::string(e.what()));
    }
}
