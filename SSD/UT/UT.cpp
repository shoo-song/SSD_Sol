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
	// CMD �� W, R �ΰ��� ����
}

TEST_F(SSDTestFixture, invalidecheck2) {
	
	// LBA 0 ~ 100 ������ ����
	int LBA = 0;
	EXPECT_THAT(LBA, Ge(0));
	EXPECT_THAT(LBA, Le(99));
}

TEST_F(SSDTestFixture, invalidecheck3) {
	// LBA 0 ~ 100 �ܴ̿� "ERROR message" 
}

TEST_F(SSDTestFixture, invalidecheck4) {
	//Data �� 0x �����Ͽ� 10��
}

TEST_F(SSDTestFixture, Read1) {
	//������ ������ 0 ���� return
}
TEST_F(SSDTestFixture, Read2) {
	//SSD_output.txt �� ������ file ����

}
TEST_F(SSDTestFixture, Read3) {
//Read ��û�� SSD_Output.txt �� write�ϰ�, read �Ͽ� write ������ ��������
}
TEST_F(SSDTestFixture, Read4) {
// ���� LBA (0~99) ���� ��, LBA �˻��Ͽ� data read
}

TEST_F(SSDTestFixture, Write1) {
// SSD_nand.txt �� ������ file ����
}

TEST_F(SSDTestFixture, Write2) {
// write data �� LBA, data �� SSD_nand.txt ����
}
TEST_F(SSDTestFixture, Write3) {
// ���ο� write cmd�� ������ data ���� write 
}
TEST_F(SSDTestFixture, Write4) {
// ���� LBA write�� LBA �˻� ��, data ���� (overwrite�̳�, ��ġ�� ����)
}

#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
