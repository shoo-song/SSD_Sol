#include "ssddriver_exception.h"

#include <exception>
#include <string>

SsdDriverException::SsdDriverException(const std::string& msg) : message(msg) {}

const char* SsdDriverException::what() const noexcept { return message.c_str(); }

SsdExecuteException::SsdExecuteException() : SsdDriverException("SsdExecuteException") {}

FileNotFoundException::FileNotFoundException() : SsdDriverException("SsdExecuteException") {}

InvalidInputException::InvalidInputException() : SsdDriverException("SsdExecuteException") {}
