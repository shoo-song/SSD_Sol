#include <gmock/gmock.h>
#include <filesystem>
#include <cstdlib>   // system()
#include <cstdio>    // remove()
#include <memory>    // std::unique_ptr
#include <sys/stat.h> // mkdir(), stat()

#include "../FileManager.h"
#include "../SSD.cpp"
#include "../command_parser.h"
#include "../BufferCommand.h"
#include "../FileSystem.h"
using namespace testing;
#define UNIT_TEST
class SSDTestFixture : public Test
{
public:
	SSD MySSD;
	FileManager FileMgr;
	CommandParser InputParser;
	FileSystem filesystem;
};

class MockFile : public FileManager {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};

TEST_F(SSDTestFixture, Read2) {
//Read 요청을 SSD_Output.txt 에 write하고, read 하여 write 내용이 읽혀야함
	std::string data1 = "0xABABABAB";
	MySSD.DoWrite(0, data1);
	EXPECT_EQ(data1, FileMgr.getReadDataFromOutput());
}
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

	FileManager file;
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
	InputParser.parseArg('s', "0", data);

	FileManager file;
	string actual = file.getReadDataFromOutput();

	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBALenght) {
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA length
	InputParser.parseArg('W', "1a", data);

	FileManager file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBARange) {
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA range
	InputParser.parseArg('W', "300", data);

	FileManager file;
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
	InputParser.parseArg('W', "3", data1);

	FileManager file;
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
	InputParser.parseArg('W', "3", data1);
	FileManager file;
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
	EXPECT_TRUE(filesystem.fileExists(testFile)) << "empty.txt 파일이 존재하지 않음";

	filesystem.createFile(true);
	EXPECT_TRUE("0_empty", filesystem.fileExists("0_empty"));
	EXPECT_TRUE("1_empty", filesystem.fileExists("1_empty"));
	EXPECT_TRUE("2_empty", filesystem.fileExists("2_empty"));
	EXPECT_TRUE("3_empty", filesystem.fileExists("3_empty"));
	EXPECT_TRUE("4_empty", filesystem.fileExists("4_empty"));
}

TEST_F(SSDTestFixture, UpdateFileName)
{
	// given : initialize output file	
}
TEST_F(SSDTestFixture, CMDMergeTest)
{
	CMDBuffer temp;
	SSDCommand cmd;
	//Erase 10~12
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 10;
	cmd.EraseEndLBA = 12;
	temp.AppendCMD(cmd);
	//Write 14
	cmd.CMDType = CMD_WRITE;
	cmd.LBA = 14;
	strcpy_s(cmd.input_data, "0xABABABAB");
	temp.AppendCMD(cmd);
	//Erase 11~15
	cmd.CMDType = CMD_ERASE;
	cmd.LBA = 11;
	cmd.EraseEndLBA = 15;
	temp.AppendCMD(cmd);
	EXPECT_EQ(1, temp.CheckValidCmdCount());
}
#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
