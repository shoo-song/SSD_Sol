#include "ssddriver.h"
#include "ssddriver_exception.cpp"
#include "logger_exception.cpp"
#include "mock_ssddriver.h"
#include <gmock/gmock.h>

class SsdDriverFixture : public testing::Test
{
public:
    SsdDriver ssdDriver;
};

class SsdDriverMockFixture : public testing::Test
{
public:
    MockSsdDriver mockssdDriver;
};

TEST_F(SsdDriverFixture, ssddrive_read_mock) {

    ssdDriver.writeSSD(1, 0x12345678);
    
    uint32_t expected = ssdDriver.readSSD(1);
    EXPECT_EQ(expected, 0x12345678);
}

TEST_F(SsdDriverMockFixture, ssddrive_read_cmd) {
    
    const std::string expected = "SSD.exe R 1";

    EXPECT_CALL(mockssdDriver, executeCmd(expected))
        .Times(1)
        .WillOnce(::testing::Return(true));

    const std::string commandStr = "SSD.exe R 1";
    bool result = mockssdDriver.executeCmd(commandStr);
    EXPECT_TRUE(result);

}

TEST_F(SsdDriverMockFixture, ssddrive_write_cmd) {

    const std::string expected = "SSD.exe W 1 0XFFFFFFFF";

    EXPECT_CALL(mockssdDriver, executeCmd(expected))
        .Times(1)
        .WillOnce(::testing::Return(true));

    const std::string commandStr = "SSD.exe W 1 0XFFFFFFFF";
    bool result = mockssdDriver.executeCmd(commandStr);
    EXPECT_TRUE(result);

}

TEST_F(SsdDriverMockFixture, ssddrive_erase_cmd) {

    const std::string expected = "SSD.exe E 0 100";

    EXPECT_CALL(mockssdDriver, executeCmd(expected))
        .Times(1)
        .WillOnce(::testing::Return(true));

    const std::string commandStr = "SSD.exe E 0 100";
    bool result = mockssdDriver.executeCmd(commandStr);
    EXPECT_TRUE(result);

}


TEST_F(SsdDriverFixture, ssd_read_excpetion) {
    EXPECT_THROW(ssdDriver.readSSD(200), SsdDriverException);
} 

TEST_F(SsdDriverFixture, ssd_write_excpetion) {
    EXPECT_THROW(ssdDriver.writeSSD(100, 0x12345678), SsdDriverException);
}
