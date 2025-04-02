#include <gmock/gmock.h>
#include "../File_Interface.cpp"
#include "../SSD.cpp"
using namespace testing;

class SSDTestFixture : public Test
{
public:
	SSDReadDriver SSDRead;
	SSDWriteDriver SSDWrite;
	InputData InputParser;
};

class MockFile : public FileInterface {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};
TEST_F(SSDTestFixture, invalidecheck1) {
	//Invalid cmd type
	char write_data[20] = "0x123456789";
	EXPECT_THROW(InputParser.ReadInput('A', "10", write_data), std::exception);
}
TEST_F(SSDTestFixture, invalidecheck2) {
	//Invalid lba range
	char write_data[20] = "0x123456789";
	EXPECT_THROW(InputParser.ReadInput('r', "100", write_data), std::exception);
}
TEST_F(SSDTestFixture, invalidecheck3) {
	//Invalid data size
	char write_data[20] = "0x123456789";
	EXPECT_THROW(InputParser.ReadInput('A', "10", write_data), std::exception);
}
TEST_F(SSDTestFixture, invalidecheck4) {
	//Invalid data format
	char write_data[20] = "0x1x345678";
	EXPECT_THROW(InputParser.ReadInput('A', "10", write_data), std::exception);
}
TEST_F(SSDTestFixture, Read1) {
	//적은적 없으면 0 으로 return
	std::string data1 = "0x00000000";
	EXPECT_EQ(data1, SSDRead.DoRead(0)); 
}
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
#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
