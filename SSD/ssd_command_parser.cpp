#include "ssd_command_parser.h"

#include <io.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

bool CommandParser::printError() {
  FileObj->writeInvalidLog();
  return false;
}

bool CommandParser::isCmdTypeValid(const char &CMD) {
  static const std::unordered_set<char> validCmds = {'w', 'r', 'e', 'f'};
  return validCmds.count(CMD) ? true : printError();
}

bool CommandParser::isInvalidCommand(size_t pos, std::string &LBAstring,
                                     uint32_t LBA) {
  bool isLengthMismatch = (pos != LBAstring.length());
  bool isOutOfRange = (LBA >= MAX_LBA_COUNT);

  if (isLengthMismatch || isOutOfRange) {
    return printError();
  }

  return true;
}

bool CommandParser::isEraseSizeValid(int argCount, char *data, uint32_t LBA) {
  if (argCount != 4) {
    return printError();
  }
  int EraseCount = stoi(data);
  bool isOutOfRange = (EraseCount < 0 || EraseCount > MAX_ERASE_SIZE);
  bool isOverflow = (LBA + EraseCount > MAX_LBA_COUNT);

  if (isOutOfRange || isOverflow) {
    return printError();
  }
  return true;
}

bool CommandParser::isWriteDataValid(int argCount, char *data, size_t &pos) {
  if (argCount != 4) {
    return printError();
  }
  std::string firstTwo(data, 2);
  if (strlen(data) != 10 || firstTwo != "0x") {
    return printError();
  }
  uint32_t LBA = std::stoul(data, &pos, 16);
  if (pos != strlen(data)) {
    return printError();
  }
  return true;
}

bool CommandParser::checkInvalidity(int argCount, const char &CMD,
                                    string LBAstring, char *data) {
  char cmdChar = std::tolower(CMD);

  if (isCmdTypeValid(cmdChar) != true) return false;
  if (cmdChar == 'f') return true;

  size_t pos = 0;
  uint32_t LBA = std::stoi(LBAstring, &pos, 10);

  if (!isInvalidCommand(pos, LBAstring, LBA)) return false;

  if (cmdChar == 'w') {
    if (isWriteDataValid(argCount, data, pos) != true) return false;
  } else if (cmdChar == 'e') {
    if (isEraseSizeValid(argCount, data, LBA) != true) return false;
  }
  return true;
}
string CommandParser::toTwoDigitString(unsigned int value) {
  ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << value;
  return oss.str();
}
CmdInfo CommandParser::parseArg(int argCount, char CMD, string LBAstring,
                                char *data) {
  CmdInfo Command;
  Command.IsValid = false;

  char cmdChar = std::tolower(CMD);
  const int lbaValue = LBAstring.empty() ? 0 : std::stoi(LBAstring);

  Command.LBAString = toTwoDigitString(lbaValue);

  if (!checkInvalidity(argCount, CMD, LBAstring, data)) {
    return Command;
  }

  Command.LBA = lbaValue;

  if (cmdChar == 'w') {
    strcpy_s(Command.input_data, data);
    Command.CMDType = CMD_WRITE;
  }
  if (cmdChar == 'r') {
    Command.CMDType = CMD_READ;
  }
  if (cmdChar == 'e') {
    Command.EraseEndLBA = Command.LBA + stoi(data) - 1;
    strcpy_s(Command.input_data, data);
    Command.CMDType = CMD_ERASE;
  }
  if (cmdChar == 'f') {
    Command.CMDType = CMD_FLUSH;
  }

  Command.IsValid = true;
  return Command;
}
