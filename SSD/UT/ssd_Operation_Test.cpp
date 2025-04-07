#pragma once
#include <gmock/gmock.h>
#include <sys/stat.h>  // mkdir(), stat()

#include <cstdio>   // remove()
#include <cstdlib>  // system()
#include <memory>   // std::unique_ptr
#include <string>

#include "../ssd_buffer_command.h"
#include "../ssd_command_executor.h"
#include "../ssd_command_file_system.h"
#include "../ssd_data_file_system.h"
#include "../ssd.cpp"
#include "../ssd_command_parser.h"
#include "iostream"
using namespace testing;
// #define UNIT_TEST
class SSDTestFixture : public Test {
 public:
  SSD MySSD;
  DataFileSystem FileMgr;
  CommandParser InputParser;

  CommandFileSystem fs;
  CmdInfo cmd;
  std::vector<CmdInfo> cmdList;
};

TEST_F(SSDTestFixture, WriteAndRead) {
  // 임이 LBA (0~99) 수행 시, LBA 검색하여 data read
  std::string data1 = "0xABABABAB";
  int LBA = 10;
  MySSD.doWrite(LBA, data1);
  MySSD.doRead(LBA);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, ReadAfterWrite) {
  std::string data1 = "0x12345678";
  MySSD.doWrite(0x0, data1);
  MySSD.doRead(0x0);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, WriteSameLBA) {
  std::string data1 = "0x12345678";
  MySSD.doWrite(0x0, data1);

  std::string data2 = "0x15678000";
  MySSD.doWrite(0x1, data2);

  data1 = "0x87654321";
  MySSD.doWrite(0x0, data1);

  data2 = "0x12343210";
  MySSD.doWrite(0x1, data2);
  MySSD.doRead(0);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
  MySSD.doRead(1);
  EXPECT_EQ(data2, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, CheckReadFile) {
  std::string data1 = "0x12345678";
  MySSD.doWrite(0x0, data1);
  MySSD.doRead(0);

  DataFileSystem file;
  EXPECT_EQ(data1, file.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, FullWriteTest) {
  std::string data1 = "0xABABABAB";
  for (int LBA = 0; LBA < 100; LBA++) {
    MySSD.doWrite(LBA, data1);
  }
  MySSD.doRead(0);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, FullReadTest) {
  std::string data1 = "0xABABABAB";
  for (int LBA = 0; LBA < 100; LBA++) {
    MySSD.doWrite(LBA, data1);
  }
  for (int LBA = 0; LBA < 100; LBA++) {
    MySSD.doRead(LBA);
    EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
  }
}
TEST_F(SSDTestFixture, ReadAndWriteTest) {
  MySSD.doRead(19);
  std::string data0 = FileMgr.getReadDataFromOutput();
  std::string data1 = "0x1289CDEF";
  MySSD.doWrite(20, data1);
  MySSD.doRead(20);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
  MySSD.doRead(19);
  EXPECT_EQ(data0, FileMgr.getReadDataFromOutput());
  data1 = "0xFF1100AA";
  MySSD.doWrite(10, data1);
  MySSD.doRead(10);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, EraseAndRead) {
  std::string data1 = "0x12345678";
  MySSD.doWrite(0x0, data1);
  MySSD.doErase(0, 1);
  MySSD.doRead(0x0);
  EXPECT_EQ("0x00000000", FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, CMDMergeTest1) {
  // Merge 3 to 1
  // Erase 10~12
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.LBAString = "10";
  cmd.EraseEndLBA = 12;
  strcpy_s(cmd.input_data, "3");
  temp.pushCommand(cmd);
  temp.initCmdList();

  // Write 14
  cmd.CMDType = CMD_WRITE;
  cmd.LBA = 14;
  cmd.LBAString = "14";
  strcpy_s(cmd.input_data, "0xABABABAB");
  temp.pushCommand(cmd);
  temp.initCmdList();
  // Erase 11~15
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 11;
  cmd.EraseEndLBA = 15;
  cmd.LBAString = "11";
  strcpy_s(cmd.input_data, "5");
  temp.pushCommand(cmd);
  EXPECT_EQ(1, temp.checkValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest2) {
  // Merge erase range, start lba same
  // Erase 10~12
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.EraseEndLBA = 12;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "3");
  temp.pushCommand(cmd);
  temp.initCmdList();
  // Erase 10~15
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "6");
  cmd.EraseEndLBA = 15;
  temp.pushCommand(cmd);
  EXPECT_EQ(1, temp.checkValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest3) {
  // Merge erase range, end lba same
  // Erase 10~15
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.EraseEndLBA = 15;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "6");
  temp.pushCommand(cmd);
  temp.initCmdList();
  // Erase 11~15
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 14;
  cmd.EraseEndLBA = 15;
  cmd.LBAString = "14";
  strcpy_s(cmd.input_data, "2");
  temp.pushCommand(cmd);
  EXPECT_EQ(1, temp.checkValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest4) {
  // Merge erase range, exceed erase size
  // Erase 10~15
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.EraseEndLBA = 15;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "6");
  temp.pushCommand(cmd);
  temp.initCmdList();
  // Erase 16~23
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 16;
  cmd.EraseEndLBA = 23;
  cmd.LBAString = "16";
  strcpy_s(cmd.input_data, "8");
  temp.pushCommand(cmd);
  EXPECT_EQ(2, temp.checkValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest5) {
  // Merge erase range, split range
  // Erase 10~15
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 10;
  cmd.EraseEndLBA = 15;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "6");
  temp.pushCommand(cmd);
  temp.initCmdList();
  // Erase 20~23
  cmd.CMDType = CMD_ERASE;
  cmd.LBA = 20;
  cmd.EraseEndLBA = 23;
  cmd.LBAString = "20";
  strcpy_s(cmd.input_data, "4");
  temp.pushCommand(cmd);
  EXPECT_EQ(2, temp.checkValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest6) {
  // Merge same LBA write
  // write LBA 10
  BufferCommand temp(fs);
  temp.initDir();
  cmd.IsValid = true;
  cmd.CMDType = CMD_WRITE;
  cmd.LBA = 10;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "0x12345678");
  temp.pushCommand(cmd);
  temp.initCmdList();

  // write LBA 10
  cmd.CMDType = CMD_WRITE;
  cmd.LBA = 10;
  cmd.LBAString = "10";
  strcpy_s(cmd.input_data, "0xABABABAB");
  temp.pushCommand(cmd);
  temp.initCmdList();

  EXPECT_EQ(true, fs.fileExists("0_W_10_0xABABABAB"));
}
TEST_F(SSDTestFixture, CMDMergeTest7) {
  // Merge same LBA write
  // Erase LBA 8 ~ 10
  BufferCommand temp(fs);
  temp.initDir();

  char data[20] = "1";
  cmd = InputParser.parseArg(4, 'E', "8", data);
  temp.pushCommand(cmd);
  temp.initCmdList();

  // write LBA 10
  cmd.CMDType = CMD_WRITE;
  cmd.LBA = 8;
  cmd.LBAString = "8";
  strcpy_s(cmd.input_data, "0xABABABAB");
  temp.pushCommand(cmd);
  temp.initCmdList();

  EXPECT_EQ(true, fs.fileExists("0_W_08_0xABABABAB"));
}
TEST_F(SSDTestFixture, bufferFlush) {
  // given : initialize output file
  fs.removeDirectory("buffer");
  fs.createDirectory();
  fs.createFiles();
  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  BufferCommand buffer(fs);

  for (int i = 0; i < 5; i++) {
    CmdInfo cmd;
    cmd.CMDType = 'W';
    cmd.LBA = i;
    cmd.LBAString = InputParser.toTwoDigitString(stoi(to_string(i)));
    cmd.IsValid = true;
    strcpy_s(cmd.input_data, "0x43211234");

    cmdList.push_back(cmd);
    buffer.pushCommand(cmd);
    buffer.initCmdList();
  }

  // 6번쨰 CMD
  int LBA = 0;
  CmdInfo cmd;
  cmd.CMDType = 'W';
  cmd.LBA = LBA;
  cmd.LBAString = to_string(LBA);
  cmd.IsValid = true;
  strcpy_s(cmd.input_data, "0x87654321");

  cmdList.push_back(cmd);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  // NAND에서 확인
  std::string data1 = "0x43211234";
  MySSD.doRead(LBA);
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, flushCommand) {
  // given : initialize output file
  fs.removeDirectory("buffer");
  fs.createDirectory();
  fs.createFiles();
  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  BufferCommand buffer(fs);

  for (int i = 10; i < 12; i++) {
    cmd.CMDType = 'W';
    cmd.LBA = i;
    cmd.LBAString = InputParser.toTwoDigitString(stoi(to_string(i)));
    cmd.IsValid = true;
    strcpy_s(cmd.input_data, "0xAAfffBBB");
    string newName = "0_W_0xAAfffBBB";
    cmdList.push_back(cmd);
    buffer.pushCommand(cmd);
    buffer.initCmdList();
  }

  char data[20] = "1";
  cmd = InputParser.parseArg(4, 'E', "10", data);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  int LBA = 10;
  cmd;
  cmd.CMDType = 'F';
  ;
  cmd.IsValid = true;

  cmdList.push_back(cmd);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  EXPECT_EQ(true, fs.fileExists("0_empty"));
  EXPECT_EQ(true, fs.fileExists("1_empty"));
  EXPECT_EQ(true, fs.fileExists("2_empty"));
  EXPECT_EQ(true, fs.fileExists("3_empty"));
  EXPECT_EQ(true, fs.fileExists("4_empty"));
}
TEST_F(SSDTestFixture, fastRead) {
  // given : initialize output file
  fs.removeDirectory("buffer");
  fs.createDirectory();
  fs.createFiles();
  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  BufferCommand buffer(fs);

  for (int i = 10; i < 12; i++) {
    CmdInfo cmd;
    cmd.CMDType = 'W';
    cmd.LBA = i;
    cmd.LBAString = InputParser.toTwoDigitString(stoi(to_string(i)));
    cmd.IsValid = true;
    strcpy_s(cmd.input_data, "0xAAAABBBB");
    string newName = "0_W_0xAAAABBBB";
    cmdList.push_back(cmd);
    buffer.pushCommand(cmd);
    buffer.initCmdList();
  }
  // 3번쨰 CMD : fast read test
  int LBA = 10;
  CmdInfo cmd;
  cmd.CMDType = 'R';
  cmd.LBA = LBA;
  cmd.LBAString = InputParser.toTwoDigitString(stoi(to_string(LBA)));
  cmd.IsValid = true;

  cmdList.push_back(cmd);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  // NAND에서 확인
  std::string data1 = "0xAAAABBBB";
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, fastReadEraseCMD) {
  // given : initialize output file
  fs.removeDirectory("buffer");
  fs.createDirectory();
  fs.createFiles();
  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  BufferCommand buffer(fs);

  char data[20] = "1";
  cmd = InputParser.parseArg(4, 'E', "10", data);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  // 2번쨰 CMD : fast read test
  int LBA = 10;
  CmdInfo cmd;
  cmd.CMDType = 'R';
  cmd.LBA = LBA;
  cmd.LBAString = InputParser.toTwoDigitString(stoi(to_string(LBA)));
  cmd.IsValid = true;

  cmdList.push_back(cmd);
  buffer.pushCommand(cmd);
  buffer.initCmdList();

  // NAND에서 확인
  std::string data1 = "0x00000000";
  EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}

#ifdef UNIT_TEST
int main() {
  ::testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}
#endif
