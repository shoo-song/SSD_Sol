#include <gmock/gmock.h>
#include "../SSD.cpp"

using namespace testing;
class MockFileManager : public FileInterface {
public:
	MOCK_METHOD(void, CheckAndDoFormat, ());
	MOCK_METHOD(bool, open, (const std::string& filename, bool IsFormat, bool IsOpenForWrite));
	MOCK_METHOD(bool, write, (int LBA, char* data));
	MOCK_METHOD(bool, read, (int LBA));
	MOCK_METHOD(void, close, ());
};
class SSDTestFixture : public Test
{
public:
	MockFileManager FileMock;
	SSDReadDriver Reader{ &FileMock };
	SSDWriteDriver Writer{ &FileMock };
};
TEST_F(SSDTestFixture, BasicWriteTest) {
	char data[20] = "0x00000000";
	Writer.DoWrite(0, data);
	EXPECT_CALL(FileMock, CheckAndDoFormat());
	EXPECT_CALL(FileMock, open(_, _, _));
	EXPECT_CALL(FileMock, write(_, _));
}
TEST_F(SSDTestFixture, invalidecheck1) {
	// CMD 는 W, R 두개만 존재
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

TEST_F(SSDTestFixture, Write1) {
// SSD_nand.txt 가 없으면 file 생성
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
