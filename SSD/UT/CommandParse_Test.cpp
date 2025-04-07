#pragma once
#include <gmock/gmock.h>
#include <sys/stat.h>  // mkdir(), stat()

#include <cstdio>   // remove()
#include <cstdlib>  // system()
#include <memory>   // std::unique_ptr
#include <string>

#include "../ssd_command_executor.h"
#include "iostream"

using namespace testing;
// #define UNIT_TEST
class CommandExecutorFixture : public Test {
 public:
  CommandFileSystem fs;
  CmdInfo cmd;
  SSD MySSD;
  CommandParser InputParser;
};

class MockBufferCommand : public BufferCommand {
 public:
  MockBufferCommand(CommandFileSystem &fs) : BufferCommand(fs) {}
  MOCK_METHOD(void, pushCommand, (const CmdInfo &cmdInfo), ());
};

TEST_F(CommandExecutorFixture, ParseInvalidCommand_MissingArgs) {
  char *argv[] = {(char *)"ssd"};
  int argc = 1;

  CmdInfo cmd = commandParse(argc, argv);
  EXPECT_FALSE(cmd.IsValid);
}

TEST_F(CommandExecutorFixture, ParseValidWriteCommand) {
  char *argv[] = {(char *)"ssd", (char *)"W", (char *)"10",
                  (char *)"0x12345678"};
  int argc = 4;

  CmdInfo cmd = commandParse(argc, argv);
  EXPECT_TRUE(cmd.IsValid);
  EXPECT_EQ(cmd.CMDType, 'W');
  EXPECT_EQ(cmd.LBA, 10);
  EXPECT_STREQ(cmd.input_data, "0x12345678");
}

TEST_F(CommandExecutorFixture, ExecuteCommandCallsPushCommand) {
  // given : initialize output file
  fs.removeDirectory("buffer");
  fs.createDirectory();
  fs.createFiles();

  CmdInfo cmd;
  cmd.CMDType = 'W';
  cmd.LBA = 1;
  cmd.LBAString = "1";
  strcpy_s(cmd.input_data, "0x34567890");
  cmd.IsValid = true;

  CommandFileSystem fs;
  BufferCommand bufferCmd(fs);

  executeCMD(cmd, bufferCmd);

  string filename = "0_W_1_0x34567890";
  EXPECT_EQ(true, fs.fileExists(filename));
}

TEST_F(CommandExecutorFixture, CheckInvalidCmd) {
  // when : invalid cmd
  char data[20] = "0x12345678";
  InputParser.parseArg(3, 's', "0", data);

  DataFileSystem file;
  string actual = file.getReadDataFromOutput();

  EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(CommandExecutorFixture, CheckInvalidLBALenght) {
  // given : initialize output file
  char data[20] = "0x12345678";
  MySSD.doWrite(0x0, data);
  MySSD.doRead(0x0);

  // when : invalid LBA length
  InputParser.parseArg(4, 'W', "1a", data);

  DataFileSystem file;
  string actual = file.getReadDataFromOutput();
  EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(CommandExecutorFixture, CheckInvalidLBARange) {
  // given : initialize output file
  char data[20] = "0x12345678";
  MySSD.doWrite(0x0, data);
  MySSD.doRead(0x0);

  // when : invalid LBA range
  InputParser.parseArg(4, 'W', "300", data);

  DataFileSystem file;
  string actual = file.getReadDataFromOutput();
  EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(CommandExecutorFixture, CheckInvalidDataRange1) {
  // given : initialize output file
  char data[20] = "0x12345678";
  MySSD.doWrite(0x0, data);
  MySSD.doRead(0x0);

  // when : invalid data size
  char data1[20] = "0x1234";
  InputParser.parseArg(4, 'W', "3", data1);

  DataFileSystem file;
  string actual = file.getReadDataFromOutput();
  EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(CommandExecutorFixture, CheckInvalidDataRange2) {
  // given : initialize output file
  char data[20] = "0x12345678";
  MySSD.doWrite(0x0, data);
  MySSD.doRead(0x0);

  // when : invalid data size
  char data1[20] = "0x1234544444";
  InputParser.parseArg(4, 'W', "3", data1);
  DataFileSystem file;
  string actual = file.getReadDataFromOutput();
  EXPECT_THAT(actual, StrEq(string("ERROR")));
}