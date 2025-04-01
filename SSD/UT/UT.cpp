#include <gmock/gmock.h>
#include "../SSD.cpp"
using namespace testing;
using namespace std;
class MockReader : public ReadDriver {
public:
	MOCK_METHOD(string, Read_SSD, (int LBA, int nand_fd, int result_fd), ());
};
class MockWriter : public WriteDriver {
public:
	MOCK_METHOD(int, Write_SSD, (int LBA, int nand_fd, char* data), ());
};
class SSDTestFixture : public Test
{
public:
	MockReader Reader;
	MockWriter Writer;
};


// (invalide check) Read Wrie �����
TEST_F(SSDTestFixture, invalidecheck1) {
	// CMD �� W, R �ΰ��� ����
	int value = 0;
	EXPECT_THAT(value, AnyOf(Eq('W'), Eq('R')));
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
	string result = Reader.Read_SSD(0/*LBA*/, 0, 0);
	EXPECT_EQ(result, "00000000");
}
TEST_F(SSDTestFixture, Read2) {
	//SSD_output.txt �� ������ file ����
	char filename_r[100] = "ssd_output.txt";
	//refact�ʿ�. Read_SSD�� LBA�� �޵���
	Reader.Read_SSD(0/*LBA*/, 0, 0);
	int result = _access(filename_r, O_RDWR);
	EXPECT_THAT(result, Ne(-1));

}
TEST_F(SSDTestFixture, Read3) {
//Read ��û�� SSD_Output.txt �� write�ϰ�, read �Ͽ� write ������ ��������
	char filename_r[100] = "ssd_output.txt";
	char filename_w[100] = "ssd_nand.txt";
	int LBA = 10;
	char* data = {};
	//refact�ʿ�. Write_SSD�� LBA, Data�� �޵���
	Writer.Write_SSD(LBA, 0, 0);
	string result = Reader.Read_SSD(LBA, 0, 0);
	EXPECT_EQ(result, data);
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

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
