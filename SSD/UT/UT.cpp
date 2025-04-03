#include <gmock/gmock.h>
#include "../File_Interface.h"
#include "../SSD.cpp"
#include "../command_parse.h"
using namespace testing;

class SSDTestFixture : public Test
{
public:
	SSDReadDriver SSDRead;
	SSDWriteDriver SSDWrite;
	SSDCommand InputParser;
};

class MockFile : public FileInterface {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};

TEST_F(SSDTestFixture, Read2) {
//Read 요청을 SSD_Output.txt 에 write하고, read 하여 write 내용이 읽혀야함
	std::string data1 = "0xABABABAB";
	SSDWrite.DoWrite(0, data1);
	EXPECT_EQ(data1, SSDRead.DoRead(0));
}
TEST_F(SSDTestFixture, Read3) {
// 임이 LBA (0~99) 수행 시, LBA 검색하여 data read
	std::string data1 = "0xABABABAB";
	int LBA = 10;
	SSDWrite.DoWrite(LBA, data1);
	EXPECT_EQ(data1, SSDRead.DoRead(LBA));
}

TEST_F(SSDTestFixture, ReadAfterWrite) {
	std::string data = "0x12345678";
	SSDWrite.DoWrite(0x0, data);

	EXPECT_EQ(data, SSDRead.DoRead(0x0));
}

TEST_F(SSDTestFixture, WriteSameLBA) {
	std::string data1 = "0x12345678";
	SSDWrite.DoWrite(0x0, data1);

	std::string data2 = "0x15678000";
	SSDWrite.DoWrite(0x1, data2);

	data1 = "0x87654321";
	SSDWrite.DoWrite(0x0, data1);

	data2 = "0x12343210";
	SSDWrite.DoWrite(0x1, data2);

	EXPECT_EQ(data1, SSDRead.DoRead(0));
	EXPECT_EQ(data2, SSDRead.DoRead(1));
}

TEST_F(SSDTestFixture, CheckReadFile) {
	std::string data1 = "0x12345678";
	SSDWrite.DoWrite(0x0, data1);
	SSDRead.DoRead(0);

	FileInterface file;
	EXPECT_EQ(data1, file.getReadDataFromOutput());
}
TEST_F(SSDTestFixture, FullWriteTest) {
	std::string data1 = "0xABABABAB";
	for (int LBA = 0; LBA < 100; LBA++) {
		SSDWrite.DoWrite(LBA, data1);
	}
	EXPECT_EQ(SSDRead.DoRead(0), data1);
}
TEST_F(SSDTestFixture, FullReadTest) {
	std::string data1 = "0xABABABAB";
	for (int LBA = 0; LBA < 100; LBA++) {
		EXPECT_EQ(SSDRead.DoRead(LBA), data1);
	}
}
TEST_F(SSDTestFixture, ReadAndWriteTest) {
	std::string data0 = SSDRead.DoRead(19);
	std::string data1 = "0x1289CDEF";
	SSDWrite.DoWrite(20, data1);
	EXPECT_EQ(SSDRead.DoRead(20), data1);
	EXPECT_EQ(SSDRead.DoRead(19), data0);
	data1 = "0xFF1100AA";
	SSDWrite.DoWrite(10, data1);
	EXPECT_EQ(SSDRead.DoRead(10), data1);
}

TEST_F(SSDTestFixture, CheckInvalidCmd) {
	SSDCommand input;
	// when : invalid cmd
	char data[20] = "0x12345678";
	input.parseArg('s', "0", data);

	FileInterface file;
	string actual = file.getReadDataFromOutput();

	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBALenght) {
	SSDCommand input;
	// given : initialize output file
	char data[20] = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);
	SSDRead.DoRead(0x0);

	// when : invalid LBA length
	input.parseArg('W', "1a", data);

	FileInterface file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidLBARange) {
	SSDCommand input;
	// given : initialize output file
	char data[20] = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);
	SSDRead.DoRead(0x0);

	// when : invalid LBA range
	input.parseArg('W', "300", data);

	FileInterface file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange1) {
	SSDCommand input;
	// given : initialize output file	
	char data[20] = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);
	SSDRead.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234";
	input.parseArg('W', "3", data1);

	FileInterface file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}

TEST_F(SSDTestFixture, CheckInvalidDataRange2) {
	SSDCommand input;
	// given : initialize output file	
	char data[20] = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);
	SSDRead.DoRead(0x0);

	// when : invalid data size
	char data1[20] = "0x1234544444";
	input.parseArg('W', "3", data1);

	FileInterface file;
	string actual = file.getReadDataFromOutput();
	EXPECT_THAT(actual, StrEq(string("ERROR")));
}


#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
