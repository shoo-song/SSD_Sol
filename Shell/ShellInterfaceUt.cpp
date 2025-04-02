#pragma once
#include "ShellInterface.cpp"
#include <gmock/gmock.h>
#include <vector>

using namespace std;

class ShellInterfaceFixture : public testing::Test
{
public:
    ShellInterface shellInterface;
    string result;
};

TEST_F(ShellInterfaceFixture, parseReadTest) {
    EXPECT_EQ(READ_COMMAND, ShellInterfaceUtil::getUtilObj().parse("read"));
};

TEST_F(ShellInterfaceFixture, parseWriteTest) {
    EXPECT_EQ(WRITE_COMMAND, ShellInterfaceUtil::getUtilObj().parse("write"));
};

TEST_F(ShellInterfaceFixture, parseExitTest) {
    EXPECT_EQ(EXIT_COMMAND, ShellInterfaceUtil::getUtilObj().parse("exit"));
}

TEST_F(ShellInterfaceFixture, parseHelpTest) {
    EXPECT_EQ(HELP_COMMAND, ShellInterfaceUtil::getUtilObj().parse("help"));
}

TEST_F(ShellInterfaceFixture, parseFullwriteTest) {
    EXPECT_EQ(FULLWRITE_COMMAND, ShellInterfaceUtil::getUtilObj().parse("fullwrite"));
}

TEST_F(ShellInterfaceFixture, parseFullreadTest) {
    EXPECT_EQ(FULLREAD_COMMAND, ShellInterfaceUtil::getUtilObj().parse("fullread"));
}

TEST_F(ShellInterfaceFixture, readWriteTest1) {
    MockSsdDriver mockDriver;
    shellInterface.setDriverInterface(&mockDriver);

    EXPECT_EQ("[Write] Done", shellInterface.execute("write 01 0xAAAABBBB"));
    EXPECT_EQ("[Read] LBA 00 : 0xAAAABBBB", shellInterface.execute("read 01"));
}

TEST_F(ShellInterfaceFixture, readTest1) {
    MockSsdDriver mockDriver;
    shellInterface.setDriverInterface(&mockDriver);
    EXPECT_THROW(shellInterface.convertCmdArgs(shellInterface.execute("read 200"), ShellArgConvertException);
}

TEST_F(ShellInterfaceFixture, writeTest1) {
    MockSsdDriver mockDriver;
    shellInterface.setDriverInterface(&mockDriver);
    EXPECT_THROW(shellInterface.convertCmdArgs(shellInterface.execute("write 200 200"), ShellArgConvertException);
}
