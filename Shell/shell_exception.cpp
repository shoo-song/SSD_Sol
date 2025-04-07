#include "shell_exception.h"

ShellException::ShellException(const std::string& msg) : message(msg) {}

const char* ShellException::what() const noexcept { return message.c_str(); }