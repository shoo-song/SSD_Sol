#pragma once

#include <stdexcept>
#include <string>

using std::exception;
using std::string;

class ShellArgConvertException : public std::exception {
   public:
    explicit ShellArgConvertException(const std::string& msg);
    const char* what() const noexcept override;

   private:
    string message;
};