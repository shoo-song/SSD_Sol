#pragma once
#include <gmock/gmock.h>
#include <filesystem>
#include <cstdlib>   // system()
#include <cstdio>    // remove()
#include <memory>    // std::unique_ptr
#include <sys/stat.h> // mkdir(), stat()

#include "../DataFileSystem.h"
#include "../SSD.cpp"
#include "../command_parser.h"
#include "../BufferCommand.h"
#include "../CommandFileSystem.h"
using namespace testing;
#define UNIT_TEST
class SSDTestFixture : public Test
{
public:
	SSD MySSD;
	DataFileSystem FileMgr;
	CommandParser InputParser;
	CommandFileSystem filesystem;
	BufferCommand temp;
	CmdInfo cmd;
};

class MockFile : public DataFileSystem {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};

TEST_F(SSDTestFixture, Read3) {
// 임이 LBA (0~99) 수행 시, LBA 검색하여 data read
	std::string data1 = "0xABABABAB";
	int LBA = 10;
	MySSD.DoWrite(LBA, data1);
	MySSD.DoRead(LBA);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}

TEST_F(SSDTestFixture, ReadAfterWrite) {
	std::string data1 = "0x12345678";
	MySSD.DoWrite(0x0, data1);
	MySSD.DoRead(0x0);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}

TEST_F(SSDTestFixture, WriteSameLBA) {
	std::string data1 = "0x12345678";
	MySSD.DoWrite(0x0, data1);

	std::string data2 = "0x15678000";
	MySSD.DoWrite(0x1, data2);

	data1 = "0x87654321";
	MySSD.DoWrite(0x0, data1);

	data2 = "0x12343210";
	MySSD.DoWrite(0x1, data2);
	MySSD.DoRead(0);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
	MySSD.DoRead(1);
	EXPECT_EQ(data2, FileMgr.getReadDataFromOutput());
}

TEST_F(SSDTestFixture, CheckReadFile) {
	std::string data1 = "0x12345678";
	MySSD.DoWrite(0x0, data1);
	MySSD.DoRead(0);

	DataFileSystem file;
	EXPECT_EQ(data1, file.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, FullWriteTest) {
	std::string data1 = "0xABABABAB";
	for (int LBA = 0; LBA < 100; LBA++) {
		MySSD.DoWrite(LBA, data1);
	}
	MySSD.DoRead(0);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, FullReadTest) {
	std::string data1 = "0xABABABAB";
	for (int LBA = 0; LBA < 100; LBA++) {
		MySSD.DoWrite(LBA, data1);
	}
	for (int LBA = 0; LBA < 100; LBA++) {
		MySSD.DoRead(LBA);
		EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
	}
}
TEST_F(SSDTestFixture, ReadAndWriteTest) {
	MySSD.DoRead(19);
	std::string data0 = FileMgr.getReadDataFromOutput();
	std::string data1 = "0x1289CDEF";
	MySSD.DoWrite(20, data1);
	MySSD.DoRead(20);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
	MySSD.DoRead(19);
	EXPECT_EQ(data0, FileMgr.getReadDataFromOutput());
	data1 = "0xFF1100AA";
	MySSD.DoWrite(10, data1);
	MySSD.DoRead(10);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}

TEST_F(SSDTestFixture, CheckInvalidCmd) {
	// when : invalid cmd
	char data[20] = "0x12345678";
	InputParser.parseArg(4, 's', "0", data);

	DataFileSystem file;
	string actual = file.getReadDataFromOutput();

	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBALenght) {
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA length
	InputParser.parseArg(4, 'W', "1a", data);

	DataFileSystem file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBARange) {
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA range
	InputParser.parseArg(4, 'W', "300", data);

	DataFileSystem file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange1) {
	// given : initialize output file	
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234";
	InputParser.parseArg(4, 'W', "3", data1);

	DataFileSystem file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange2) {
	// given : initialize output file	
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234544444";
	InputParser.parseArg(4, 'W', "3", data1);
	DataFileSystem file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}
TEST_F(SSDTestFixture, EraseAndRead) {
	std::string data1 = "0x12345678";
	MySSD.DoWrite(0x0, data1);
	MySSD.DoErase(0, 1);
	MySSD.DoRead(0x0);
	EXPECT_EQ("0x00000000", FileMgr.getReadDataFromOutput());
}

TEST_F(SSDTestFixture, CMDMergeTest1)
{
	// Merge 3 to 1
	//Erase 10~12
	temp.InitDir();
	cmd.IsValid = true;
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.LBAString = "10";
	cmd.EraseEndLBA = 12;
	strcpy_s(cmd.input_data, "3");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Write 14
	cmd.CMDType = CMD_WRITE;
	cmd.LBA = 14;
	cmd.LBAString = "14";
	strcpy_s(cmd.input_data, "0xABABABAB");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Erase 11~15
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 11;
	cmd.EraseEndLBA = 15;
	cmd.LBAString = "11";
	strcpy_s(cmd.input_data, "5");
	temp.PushCommand(cmd);
	EXPECT_EQ(1, temp.CheckValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest2)
{
	// Merge erase range, start lba same
	//Erase 10~12
	temp.InitDir();
	cmd.IsValid = true;
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.EraseEndLBA = 12;
	cmd.LBAString = "10";
	strcpy_s(cmd.input_data, "3");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Erase 10~15
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.LBAString = "10";
	strcpy_s(cmd.input_data, "6");
	cmd.EraseEndLBA = 15;
	temp.PushCommand(cmd);
	EXPECT_EQ(1, temp.CheckValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest3)
{
	// Merge erase range, end lba same
	//Erase 10~15
	temp.InitDir();
	cmd.IsValid = true;
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.EraseEndLBA = 15;
	cmd.LBAString = "10";
	strcpy_s(cmd.input_data, "6");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Erase 11~15
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 14;
	cmd.EraseEndLBA = 15;
	cmd.LBAString = "14";
	strcpy_s(cmd.input_data, "2");
	temp.PushCommand(cmd);
	EXPECT_EQ(1, temp.CheckValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest4)
{
	// Merge erase range, exceed erase size
	//Erase 10~15
	temp.InitDir();
	cmd.IsValid = true;
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.EraseEndLBA = 15;
	cmd.LBAString = "10";
	strcpy_s(cmd.input_data, "6");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Erase 16~23
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 16;
	cmd.EraseEndLBA = 23;
	cmd.LBAString = "16";
	strcpy_s(cmd.input_data, "8");
	temp.PushCommand(cmd);
	EXPECT_EQ(2, temp.CheckValidCmdCount());
}
TEST_F(SSDTestFixture, CMDMergeTest5)
{
	// Merge erase range, split range
	//Erase 10~15
	temp.InitDir();
	cmd.IsValid = true;
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.EraseEndLBA = 15;
	cmd.LBAString = "10";
	strcpy_s(cmd.input_data, "6");
	temp.PushCommand(cmd);
	temp.InitCmdList();
	//Erase 20~23
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 20;
	cmd.EraseEndLBA = 23;
	cmd.LBAString = "20";
	strcpy_s(cmd.input_data, "4");
	temp.PushCommand(cmd);
	EXPECT_EQ(2, temp.CheckValidCmdCount());
}
TEST_F(SSDTestFixture, CreateBufferFolder) {
	std::string testDir = "buffer";
	// 1️. 폴더 생성
	filesystem.createDirectory();
	EXPECT_TRUE(filesystem.directoryExists(testDir)) << "폴더 생성 실패";

	// 2️. 폴더 삭제
	filesystem.removeDirectory(testDir);
	EXPECT_FALSE(filesystem.directoryExists(testDir)) << "폴더 삭제 실패";
}

TEST_F(SSDTestFixture, CreateEmptyFiles) {
	// given : create buffer directory
	filesystem.createDirectory();

	std:string testFile = "empty";
	EXPECT_FALSE( filesystem.fileExists(testFile)) << "empty.txt 파일이 존재하지 않음";

	filesystem.createFiles();
	EXPECT_EQ(true, filesystem.fileExists("0_empty"));
	EXPECT_EQ(true, filesystem.fileExists("1_empty"));
	EXPECT_EQ(true, filesystem.fileExists("2_empty"));
	EXPECT_EQ(true, filesystem.fileExists("3_empty"));
	EXPECT_EQ(true, filesystem.fileExists("4_empty"));
}	

TEST_F(SSDTestFixture, MakeCmdListFromBufferFiles)
{
	// given : initialize output file	
	filesystem.createFiles();

	std::vector<std::string> fileNames;
	fileNames = filesystem.makeCmdList();

	std::vector<std::string> expectedFiles = { "0_empty", "1_empty", "2_empty", "3_empty", "4_empty" };
	for (auto name : fileNames) {
		EXPECT_THAT(name, AnyOfArray(expectedFiles));
	}
}

TEST_F(SSDTestFixture, updateFileName)
{
	// given : initialize output file	
	filesystem.createFiles();

	std::vector<std::string> fileNames;
	fileNames = filesystem.makeCmdList();

	string newName = "0_W_0_0x12345678";
	string oldName = fileNames[0];
	filesystem.updateFileName(oldName, newName);
	EXPECT_TRUE(true, filesystem.fileExists(newName));
}

TEST_F(SSDTestFixture, updateCmdListAndFileName)
{
	// given : initialize output file	
	CommandFileSystem fs;
	fs.removeDirectory("buffer");
	fs.createDirectory();
	fs.createFiles();
	std::vector<std::string> fileNames;
	fileNames = fs.makeCmdList();

	BufferCommand buffer;
	std::vector<CmdInfo> cmdList;

	for (int i = 0; i < 5; i++) {
		CmdInfo cmd = { 'W' , i, to_string(i), "0x12345678" };
		cmdList.push_back(cmd);
		buffer.PushCommand(cmd);
	}

	int idx = 0;
	for (auto cmd : cmdList) {
		string newFileName = to_string(idx) + "_" +
			std::string(1, cmd.CMDType) + "_" +
			cmd.LBAString + "_" +
			cmd.input_data;

		EXPECT_TRUE(true, file.fileExists(newFileName));
	}
}


TEST_F(SSDTestFixture, bufferFlush)
{
	// given : initialize output file	
	CommandFileSystem fs;
	fs.removeDirectory("buffer");
	fs.createDirectory();
	fs.createFiles();
	std::vector<std::string> fileNames;
	fileNames = fs.makeCmdList();

	BufferCommand buffer;
	std::vector<CmdInfo> cmdList;

	for (int i = 0; i < 5; i++) {
		CmdInfo cmd = { 'W' , i, to_string(i), "0x43211234" };
		cmdList.push_back(cmd);
		buffer.PushCommand(cmd);
	}

	// 6번쨰 CMD
	int LBA = 0;
	CmdInfo cmd = { 'W', 0, to_string(LBA), "0x87654321" };
	cmdList.push_back(cmd);
	buffer.PushCommand(cmd);

	// NAND에서 확인
	std::string data1 = "0x43211234";	
	MySSD.DoRead(LBA);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());

	// empty file 확인
	EXPECT_TRUE(true, filesystem.fileExists("0_empty"));
}
TEST_F(SSDTestFixture, extractCMDfromFile)
{
	// given : initialize output file	
	CommandFileSystem fs;
	fs.removeDirectory("buffer");
	fs.createDirectory();
	fs.createFiles();
	std::vector<std::string> fileNames;
	fileNames = fs.makeCmdList();

	BufferCommand buffer;
	std::vector<CmdInfo> cmdList;

	for (int i = 0; i < 5; i++) {
		CmdInfo cmd = { 'W' , i, to_string(i), "0xAAAABBBB" };
		string newName = "0_W_0xAAAABBBB";
		cmdList.push_back(cmd);
		buffer.PushCommand(cmd);
	}
	// 6번쨰 CMD
	int LBA = 0;
	CmdInfo cmd = { 'R' , 0, to_string(LBA) };
	cmdList.push_back(cmd);
	buffer.PushCommand(cmd);

	// NAND에서 확인
	std::string data1 = "0xAAAABBBB";
	MySSD.DoRead(LBA);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}

#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
