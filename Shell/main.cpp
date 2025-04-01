#include <gmock/gmock.h>

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