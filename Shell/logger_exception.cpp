#pragma once
#include <stdexcept>
#include <string>

class LoggerException : public std::runtime_error {
   public:
    explicit LoggerException(const std::string& message) : std::runtime_error(message) {}
};