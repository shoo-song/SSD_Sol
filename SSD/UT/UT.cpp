#include <gmock/gmock.h>
#include "../FileManager.h"
#include "../SSD.cpp"
#include "../command_parse.h"
using namespace testing;

class SSDTestFixture : public Test
{
public:
	SSD MySSD;
	FileManager FileMgr;
	SSDCommand InputParser;
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
	SSDCommand input;
	// when : invalid cmd
	char data[20] = "0x12345678";
	input.parseArg('s', "0", data);

	FileManager file;
	string actual = file.getReadDataFromOutput();

	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBALenght) {
	SSDCommand input;
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA length
	input.parseArg('W', "1a", data);

	FileManager file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBARange) {
	SSDCommand input;
	// given : initialize output file
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid LBA range
	input.parseArg('W', "300", data);

	FileManager file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange1) {
	SSDCommand input;
	// given : initialize output file	
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234";
	input.parseArg('W', "3", data1);

	FileManager file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange2) {
	SSDCommand input;
	// given : initialize output file	
	char data[20] = "0x12345678";
	MySSD.DoWrite(0x0, data);
	MySSD.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234544444";
	input.parseArg('W', "3", data1);

	FileManager file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}


#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
