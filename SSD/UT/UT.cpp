#include <gmock/gmock.h>

using namespace testing;

class SSDTestFixture : public Test
{
public:
	
};


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


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}