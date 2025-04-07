#pragma once

#include <stdexcept>
#include <string>

using std::exception;
using std::string;

class ShellException : public std::exception {
   public:
    explicit ShellException(const std::string& msg);
    const char* what() const noexcept override;

   private:
    string message;
};