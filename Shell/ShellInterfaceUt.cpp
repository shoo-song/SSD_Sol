#pragma once
#include "ShellExecutor.cpp"
#include "mock_ssddriver.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>
#include <unordered_map>

using namespace testing;
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
    EXPECT_CALL(mockDriver, writeSSD(1, 0xAAAABBBB))
        .WillOnce(testing::Return());

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

TEST_F(ShellExecutorFixture, script1_fullmatching) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(14)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) {
            memory[pos] = data;
           }));

    EXPECT_CALL(mockDriver, readSSD(::testing::_))
        .Times(14)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
        auto it = memory.find(pos);
        if (it != memory.end()) {
            return it->second;
        }
        return -1; // 해당 위치에 데이터가 없을 경우
            }));

    EXPECT_EQ("PASS", shellExecutor.execute("1_FullWriteAndReadCompare"));

}

TEST_F(ShellExecutorFixture, script1_wildcard) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(14)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) {
        memory[pos] = data;
            }));

    EXPECT_CALL(mockDriver, readSSD(::testing::_))
        .Times(14)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
        auto it = memory.find(pos);
        if (it != memory.end()) {
            return it->second;
        }
        return -1; // 해당 위치에 데이터가 없을 경우
            }));

    EXPECT_EQ("PASS", shellExecutor.execute("1_"));
}

TEST_F(ShellExecutorFixture, script2_fullmatching) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) {
        memory[pos] = data;
            }));

    EXPECT_CALL(mockDriver, readSSD(::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
        auto it = memory.find(pos);
        if (it != memory.end()) {
            return it->second;
        }
        return -1; // 해당 위치에 데이터가 없을 경우
            }));

    EXPECT_EQ("PASS", shellExecutor.execute("2_PartialLBAWrite"));

}

TEST_F(ShellExecutorFixture, script2_wildcard) {
    MockSsdDriver mockDriver;
    shellExecutor.setDriverInterface(&mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) {
        memory[pos] = data;
            }));

    EXPECT_CALL(mockDriver, readSSD(::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
        auto it = memory.find(pos);
        if (it != memory.end()) {
            return it->second;
        }
        return -1; // 해당 위치에 데이터가 없을 경우
            }));

    EXPECT_EQ("PASS", shellExecutor.execute("2_"));
}
