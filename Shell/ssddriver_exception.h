#pragma once
#include <exception>
#include <string>

class SsdDriverException : public std::exception {
   public:
    explicit SsdDriverException(const std::string& msg);
    const char* what() const noexcept override;

   protected:
    std::string message;
};

class SsdExecuteException : public SsdDriverException {
   public:
    SsdExecuteException();
};

class FileNotFoundException : public SsdDriverException {
   public:
    FileNotFoundException();
};

class InvalidInputException : public SsdDriverException {
   public:
    InvalidInputException();
};