#pragma once
#include "ShellInterface.cpp"
#include <gmock/gmock.h>
#include <vector>

using namespace std;

class ShellInterfaceFixture : public testing::Test
{
public:
    ShellInterface shellInterface;
    vector<unsigned int> expectedArgs;
    vector<string> inputArgs;
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

TEST_F(ShellInterfaceFixture, parseArgTest1) {
    vector<unsigned int> convertedArgs;

    inputArgs.push_back("01");
    expectedArgs.push_back(1);

    convertedArgs = shellInterface.convertCmdArgs(READ_COMMAND, inputArgs);

    EXPECT_EQ(expectedArgs[0], convertedArgs[0]);
    EXPECT_EQ(expectedArgs.size(), convertedArgs.size());
}

TEST_F(ShellInterfaceFixture, readCmdParseArgTest2) {
    inputArgs.push_back("200");
    EXPECT_THROW(shellInterface.convertCmdArgs(READ_COMMAND, inputArgs), ShellArgConvertException);
}

TEST_F(ShellInterfaceFixture, readCmdParseArgTest3) {
    inputArgs.push_back("11");
    inputArgs.push_back("20");
    EXPECT_THROW(shellInterface.convertCmdArgs(READ_COMMAND, inputArgs), ShellArgConvertException);
}

TEST_F(ShellInterfaceFixture, writeParseArgTest1) {
    vector<unsigned int> convertedArgs;

    inputArgs.push_back("1");
    inputArgs.push_back("0x12345678");

    expectedArgs.push_back(1);
    expectedArgs.push_back(0x12345678);

    convertedArgs = shellInterface.convertCmdArgs(WRITE_COMMAND, inputArgs);

    EXPECT_EQ(expectedArgs[0], convertedArgs[0]);
    EXPECT_EQ(expectedArgs[1], convertedArgs[1]);
    EXPECT_EQ(expectedArgs.size(), convertedArgs.size());
}
