#include <gmock/gmock.h>
#include "SSD.cpp"
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

TEST(TestScript, readTest) {

    TestScript ts;

    EXPECT_EQ("read", ts.read(argv[0]));
};

TEST(TestScript, writeTest) {

    TestScript ts;

    EXPECT_EQ("write", ts.read(argv[0]));
};


TEST(TestScript, exitTest) {

    TestScript ts;

    EXPECT_EQ("exit", ts.read(argv[0]));
};

TEST(TestScript, helpTest) {

    TestScript ts;

    EXPECT_EQ("help", ts.read(argv[0]));
};

TEST(TestScript, fullwriteTest) {

    TestScript ts;

    EXPECT_EQ("fullwrite", ts.read(argv[0]));
};

TEST(TestScript, fullreadTest) {

    TestScript ts;

    EXPECT_EQ("fullread", ts.read(argv[0]));
};

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}