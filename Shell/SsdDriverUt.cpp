#pragma once

#include <gmock/gmock.h>

class SsdDriverFixture : public testing::Test
{
public:
    SsdDriver ssdDriver;
};

TEST_F(SsdDriverFixture, ssd_readwrite_normal) {
    ssdDriver.write(1, 0x12345678);
    EXPECT_EQ(0x12345678, ssdDriver.read(1));
}

TEST_F(SsdDriverFixture, ssd_read_excpetion) {
    EXPECT_THROW(ssdDriver.read(200), SsdException);
}

TEST_F(SsdDriverFixture, ssd_write_excpetion) {
    EXPECT_THROW(ssdDriver.write(100, 0x12345678), SsdException);
}
