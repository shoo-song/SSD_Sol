#include <iostream>
#include "gmock/gmock.h"

TEST(SSD_DRIVER, ssd_readwrite_normal) {
	SsdDriver ssdDriver;
	ssdDriver.write(1, 0x12345678);
	EXPECT_EQ(0x12345678, ssdDriver.read(1));
}

TEST(SSD_DRIVER, ssd_read_excpetion) {
	SsdDriver ssdDriver;
	EXPECT_THROW(ssdDriver.read(200), SsdException);
}

TEST(SSD_DRIVER, ssd_write_excpetion) {
	SsdDriver ssdDriver;
	EXPECT_THROW(ssdDriver.write(100, 0x12345678), SsdException);
}

int main(int argc, char** argv) {
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}