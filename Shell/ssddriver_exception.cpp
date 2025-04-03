#include <string>
#include <exception>

class SsdDriverException : public std::exception {
public:

    SsdDriverException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
protected:
    std::string message;
};

class SsdExecuteException : public SsdDriverException {
public:
    SsdExecuteException() : SsdDriverException("InvalidInputException") {}
};

class FileNotFoundException : public SsdDriverException {
public:
    FileNotFoundException() : SsdDriverException("FileNotFoundException") {}
};

class InvalidInputException : public SsdDriverException {
public:
    InvalidInputException() : SsdDriverException("InvalidInputException") {}
};