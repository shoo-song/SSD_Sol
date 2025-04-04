#include "shell_exception.h"

ShellArgConvertException::ShellArgConvertException(const std::string& msg)
	: message(msg) {
}

const char* ShellArgConvertException::what() const noexcept {
	return message.c_str();
}