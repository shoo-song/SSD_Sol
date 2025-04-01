#pragma once

#include <gmock/gmock.h>

TEST(ShellInterfaceUt, parseReadTest) {
    ShellInterface shellInterface;

    EXPECT_EQ(READ_COMMAND, shellInterface.parse("read"));
};

TEST(ShellInterfaceUt, parseWriteTest) {
    ShellInterface shellInterface;

    EXPECT_EQ(WRITE_COMMAND, shellInterface.parse("write"));
};


TEST(ShellInterfaceUt, parseExitTest) {
    ShellInterface shellInterface;

    EXPECT_EQ(EXIT_COMMAND, shellInterface.parse("exit");
};

TEST(ShellInterfaceUt, parseHelpTest) {
    ShellInterface shellInterface;

    EXPECT_EQ(HELP_COMMAND, shellInterface.parse("help");
};

TEST(ShellInterfaceUt, parseFullwriteTest) {
    ShellInterface shellInterface;

    EXPECT_EQ(FULLWRITE_COMMAND, shellInterface.parse(("fullwrite"));
};

TEST(ShellInterfaceUt, parseFullreadTest) {
    ShellInterface shellInterface;

    EXPECT_EQ("fullread", tsshellInterface.parse(("fullread"));
};