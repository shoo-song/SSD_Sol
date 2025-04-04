#pragma once

#include <stdexcept>
#include <string>

using std::exception;
using std::string;

class ShellArgConvertException : public std::exception {
public:
	explicit ShellArgConvertException(const std::string& msg) : message(msg) {}

	const char* what() const noexcept override {
		return message.c_str();
	}
private:
	string message;
};