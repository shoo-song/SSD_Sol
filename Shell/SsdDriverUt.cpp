#pragma once
#include "ssddriver.h"
#include "ssddriver_exception.h"
#include "mock_ssddriver.h"
#include <gmock/gmock.h>

class SsdDriverFixture : public testing::Test
{
public:
    SsdDriver ssdDriver;
};

TEST_F(SsdDriverFixture, ssd_readwrite_normal) {

    ssdDriver.writeSSD(1, 0x12345678);
    MockSsdDriver mockssdDriver;
    EXPECT_CALL(mockssdDriver, readSSD(1))
        .WillOnce(testing::Return(0x12345678));

    
    // EXPECT_EQ(0x12345678, ssdDriver.readSSD(1));
}

TEST_F(SsdDriverFixture, ssd_read_excpetion) {
    EXPECT_THROW(ssdDriver.readSSD(200), SsdDriverException);
}   
//
//TEST_F(SsdDriverFixture, ssd_write_excpetion) {
//    EXPECT_THROW(ssdDriver.writeSSD(100, 0x12345678), SsdDriverException);
//}
