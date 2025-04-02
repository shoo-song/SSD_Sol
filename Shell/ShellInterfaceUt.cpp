#pragma once
#include "ShellExecutor.cpp"
#include "mock_ssddriver.h"
#include <gmock/gmock.h>
#include <vector>

using namespace std;

class ShellExecutorFixture : public testing::Test
{
public:
    ShellExecutor shellExecutor;
    string result;
};

TEST_F(ShellExecutorFixture, parseReadTest) {
    EXPECT_EQ(READ_COMMAND, ShellUtil::getUtilObj().parse("read"));
};

TEST_F(ShellExecutorFixture, parseWriteTest) {
    EXPECT_EQ(WRITE_COMMAND, ShellUtil::getUtilObj().parse("write"));
};

TEST_F(ShellExecutorFixture, parseExitTest) {
    EXPECT_EQ(EXIT_COMMAND, ShellUtil::getUtilObj().parse("exit"));
}

TEST_F(ShellExecutorFixture, parseHelpTest) {
    EXPECT_EQ(HELP_COMMAND, ShellUtil::getUtilObj().parse("help"));
}

TEST_F(ShellExecutorFixture, parseFullwriteTest) {
    EXPECT_EQ(FULLWRITE_COMMAND, ShellUtil::getUtilObj().parse("fullwrite"));
}

TEST_F(ShellExecutorFixture, parseFullreadTest) {
    EXPECT_EQ(FULLREAD_COMMAND, ShellUtil::getUtilObj().parse("fullread"));
}

TEST_F(ShellExecutorFixture, readWriteTest1) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    EXPECT_CALL(mockDriver, readSSD(1))
        .WillOnce(testing::Return(0xAAAABBBB));

    EXPECT_EQ("[Write] Done", shellExecutor.execute("write 01 0xAAAABBBB"));
    EXPECT_EQ("[Read] LBA 01 : 0xAAAABBBB", shellExecutor.execute("read 01"));
}

TEST_F(ShellExecutorFixture, readTest1) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    EXPECT_THROW(shellExecutor.execute("read 200"), ShellArgConvertException);
}

TEST_F(ShellExecutorFixture, writeTest1) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    EXPECT_THROW(shellExecutor.execute("write 200 200"), ShellArgConvertException);
}
