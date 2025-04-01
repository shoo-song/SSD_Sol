#pragma once
#include "ShellInterface.cpp"
#include <gmock/gmock.h>

class ShellInterfaceFixture : public testing::Test
{
public:
    ShellInterface shellInterface;
};

TEST_F(ShellInterfaceFixture, parseReadTest) {
    EXPECT_EQ(READ_COMMAND, shellInterface.parse("read"));
};

TEST_F(ShellInterfaceFixture, parseWriteTest) {
    EXPECT_EQ(WRITE_COMMAND, shellInterface.parse("write"));
};


TEST_F(ShellInterfaceFixture, parseExitTest) {
    EXPECT_EQ(EXIT_COMMAND, shellInterface.parse("exit"));
}

TEST_F(ShellInterfaceFixture, parseHelpTest) {
    EXPECT_EQ(HELP_COMMAND, shellInterface.parse("help"));
}

TEST_F(ShellInterfaceFixture, parseFullwriteTest) {
    EXPECT_EQ(FULLWRITE_COMMAND, shellInterface.parse("fullwrite"));
}

TEST_F(ShellInterfaceFixture, parseFullreadTest) {
    EXPECT_EQ(FULLREAD_COMMAND, shellInterface.parse("fullread"));
}