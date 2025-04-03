#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class Logger {
public:
    static Logger& getInstance();

    void setLogFile(const std::string& filename = "latest.log");
    void print(const std::string& location, const std::string& message);
    void closeLogFile();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;
    ~Logger();

    void openLogFile();
    void rotateIfNeeded();
    std::string getTimestampedFilename();

    std::ofstream logFile_;
    std::string logFileName_ = "latest.log";
};

extern Logger& logger;