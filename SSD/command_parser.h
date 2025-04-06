#pragma once
#include "DataFileSystem.h"
using namespace std;

#define MAX_LBA_COUNT (100)
#define MAX_ERASE_SIZE (10)

enum CommandType {
  CMD_WRITE = 'W',
  CMD_READ = 'R',
  CMD_ERASE = 'E',
  CMD_FLUSH = 'F'
};
struct CmdInfo {
  char CMDType;
  int LBA;
  string LBAString;
  char input_data[20];
  int EraseEndLBA = 0;
  bool IsValid = false;
};

class CommandParser {
 public:
  CommandParser() : FileObj(std::make_unique<DataFileSystem>()) {}

  bool checkInvalidity(int argCount, const char &CMD, string LBAstring,
                       char *data);
  string toTwoDigitString(unsigned int value);
  CmdInfo parseArg(int argCount, char CMD, string LBAstring = "",
                   char *data = nullptr);

 private:
  std::unique_ptr<DataFileSystem> FileObj;

  bool PrintError();
  bool isInvalidCommand(size_t pos, std::string &LBAstring, uint32_t LBA);

  bool isCmdTypeValid(const char &CMD);

  bool isEraseSizeValid(int argCount, char *data, uint32_t LBA);

  bool isWriteDataValid(int argCount, char *data, size_t &pos);
};
