#include "ssddriver.h"
#include "ssddriver_exception.h"
#include "mock_ssddriver.h"
#include <gmock/gmock.h>

class SsdDriverFixture : public testing::Test
{
public:
    SsdDriver ssdDriver;
};

//TEST_F(SsdDriverFixture, ssddrive_read_mock) {
//
//    
//    MockSsdDriver mockssdDriver;
//    EXPECT_CALL(mockssdDriver, readSSD(0))
//        .WillOnce(testing::Return(0xFFFFFFFF));
//    
//    uint32_t expected = ssdDriver.readSSD(0);
//    EXPECT_EQ(expected, 0xFFFFFFFF);
//}

TEST_F(SsdDriverFixture, ssddrive_read_normal_1) {

    uint32_t expected = 0xFFFFFFFF;
    EXPECT_EQ(expected, ssdDriver.readSSD(0));  

}

TEST_F(SsdDriverFixture, ssddrive_read_normal_2) {

    ssdDriver.readSSD(3);
    uint32_t expected = 0x12345678;
    EXPECT_EQ(expected, ssdDriver.readSSD(3));

}

TEST_F(SsdDriverFixture, ssddrive_read_cmd) {
    
    MockSsdDriver mockssdDriver;
    const std::string expected = "SSD.exe R 1";

    EXPECT_CALL(mockssdDriver, executeCmd(expected))
        .Times(1)
        .WillOnce(::testing::Return(true));

    const std::string commandStr = "SSD.exe R 1";
    bool result = mockssdDriver.executeCmd(commandStr);
    EXPECT_TRUE(result);

}

TEST_F(SsdDriverFixture, ssddrive_write_cmd) {

    MockSsdDriver mockssdDriver;
    const std::string expected = "SSD.exe W 1 0XFFFFFFFF";

    EXPECT_CALL(mockssdDriver, executeCmd(expected))
        .Times(1)
        .WillOnce(::testing::Return(true));

    const std::string commandStr = "SSD.exe W 1 0XFFFFFFFF";
    bool result = mockssdDriver.executeCmd(commandStr);
    EXPECT_TRUE(result);

}

/*TEST_F(SsdDriverFixture, ssd_read_excpetion) {
    EXPECT_THROW(ssdDriver.readSSD(200), SsdDriverException);
}  */ 
//
//TEST_F(SsdDriverFixture, ssd_write_excpetion) {
//    EXPECT_THROW(ssdDriver.writeSSD(100, 0x12345678), SsdDriverException);
//}
