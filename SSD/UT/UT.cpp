
#include <gmock/gmock.h>
#include "../File_Interface.cpp"
#include "../SSD.cpp"
using namespace testing;

class SSDTestFixture : public Test
{
public:
	SSDReadDriver SSDRead;
	SSDWriteDriver *SSDWrite;
	FileInterface* SSDIn;
};

class MockFile : public FileInterface {
public:
	MOCK_METHOD(bool, open, (const std::string& filename), ());
};
#if 0
TEST_F(SSDTestFixture, FileOpen) {
	MockFile file;
	EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(true));

	FileInterface ssdIO;
	EXPECT_EQ(true, ssdIO.open("ssd_nand.txt"));
}

TEST_F(SSDTestFixture, FileOpenCheck) {
	MockFile file;
	EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(true));

	FileInterface ssdIO;
	EXPECT_EQ(true, ssdIO.isOpen());
}

TEST_F(SSDTestFixture, FileClose) {
	MockFile file;
    EXPECT_CALL(file, open("ssd_nand.txt")).WillRepeatedly(Return(false));

	FileInterface ssdIO;
	ssdIO.close();

	EXPECT_EQ(false, ssdIO.isOpen());
}

// (invalide check) Read Wrie 공통부
TEST_F(SSDTestFixture, invalidecheck1) {
	// CMD 는 W, R 두개만 존재
	int value = 0;
	EXPECT_THAT(value, AnyOf(Eq('W'), Eq('R')));
}

TEST_F(SSDTestFixture, invalidecheck2) {
	
	// LBA 0 ~ 100 범위내 동작
	int LBA = 0;
	EXPECT_THAT(LBA, Ge(0));
	EXPECT_THAT(LBA, Le(99));
}

TEST_F(SSDTestFixture, invalidecheck3) {
	// LBA 0 ~ 100 이외는 "ERROR message" 
}

TEST_F(SSDTestFixture, invalidecheck4) {
	//Data 는 0x 포함하여 10자
}
#endif
TEST_F(SSDTestFixture, Read1) {
	//적은적 없으면 0 으로 return
	
}
TEST_F(SSDTestFixture, Read2) {
	//SSD_output.txt 가 없으면 file 생성

}
TEST_F(SSDTestFixture, Read3) {
//Read 요청을 SSD_Output.txt 에 write하고, read 하여 write 내용이 읽혀야함
}
TEST_F(SSDTestFixture, Read4) {
// 임이 LBA (0~99) 수행 시, LBA 검색하여 data read
}

TEST_F(SSDTestFixture, ReadAfterWrite) {
	std::string data = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data);

	EXPECT_EQ(data, SSDRead.DoRead(0x0));
}

TEST_F(SSDTestFixture, WriteSameLBA) {
	std::string data1 = "0x12345678";
	SSDWriteDriver SSDWrite;
	SSDWrite.DoWrite(0x0, data1);

	std::string data2 = "0x15678";
	SSDWrite.DoWrite(0x1, data2);

	data1 = "0x87654321";
	SSDWrite.DoWrite(0x0, data1);

	data2 = "0x1234321";
	SSDWrite.DoWrite(0x1, data2);

	//EXPECT_EQ(data1, SSDRead.DoRead(0));
	//EXPECT_EQ(data2, SSDRead.DoRead(1));


}


TEST_F(SSDTestFixture, Write2) {
// write data 는 LBA, data 로 SSD_nand.txt 저장
}
TEST_F(SSDTestFixture, Write3) {
// 새로운 write cmd는 마지막 data 이후 write 
}
TEST_F(SSDTestFixture, Write4) {
// 동일 LBA write는 LBA 검색 후, data 변경 (overwrite이나, 위치는 무관)
}

#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
