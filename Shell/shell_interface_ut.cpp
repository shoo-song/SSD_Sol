#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>
#include <memory>

#include "shell_executor.h"
#include "ssddriver_store.h"
#include "script_loader.h"

using namespace testing;
using namespace std;

class MockSsdDriverInterface : public SsdDriverInterface {
public:
    MOCK_METHOD(uint32_t, readSSD, (int LBA), (override));
    MOCK_METHOD(void, writeSSD, (int LBA, uint32_t data), (override));
    MOCK_METHOD(void, eraseSSD, (int LBA, int size), (override));
    MOCK_METHOD(void, flushSSD, (), (override));
};


class ShellExecutorFixture : public testing::Test {
   public:
    ShellExecutor shellExecutor;
    string result;
};

TEST_F(ShellExecutorFixture, parseReadTest) {
    EXPECT_EQ(SSD_READ_COMMAND, ShellUtil::getUtilObj().parse("read"));
};

TEST_F(ShellExecutorFixture, parseWriteTest) {
    EXPECT_EQ(SSD_WRITE_COMMAND, ShellUtil::getUtilObj().parse("write"));
};

TEST_F(ShellExecutorFixture, parseExitTest) {
    EXPECT_EQ(SSD_EXIT_COMMAND, ShellUtil::getUtilObj().parse("exit"));
}

TEST_F(ShellExecutorFixture, parseHelpTest) {
    EXPECT_EQ(SSD_HELP_COMMAND, ShellUtil::getUtilObj().parse("help"));
}

TEST_F(ShellExecutorFixture, parseFullwriteTest) {
    EXPECT_EQ(SSD_FULLWRITE_COMMAND, ShellUtil::getUtilObj().parse("fullwrite"));
}

TEST_F(ShellExecutorFixture, parseFullreadTest) {
    EXPECT_EQ(SSD_FULLREAD_COMMAND, ShellUtil::getUtilObj().parse("fullread"));
}

TEST_F(ShellExecutorFixture, readWriteTest1) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_CALL(*mockDriver, readSSD(1)).WillOnce(testing::Return(0xAAAABBBB));
    EXPECT_CALL(*mockDriver, writeSSD(1, 0xAAAABBBB)).WillOnce(testing::Return());

    EXPECT_EQ("[Write] Done", shellExecutor.execute("write 01 0xAAAABBBB", false));
    EXPECT_EQ("[Read] LBA 01 : 0xAAAABBBB", shellExecutor.execute("read 01", false));
}

TEST_F(ShellExecutorFixture, readTest1) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_THROW(shellExecutor.execute("read 200", false), ShellException);
}

TEST_F(ShellExecutorFixture, writeTest1) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_THROW(shellExecutor.execute("write 200 200", false), ShellException);
}

TEST_F(ShellExecutorFixture, erase_size) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_CALL(*mockDriver, eraseSSD(1, 2))
        .Times(1);
    EXPECT_EQ("[Erase] Done", shellExecutor.execute("erase 01 2", false));
}

TEST_F(ShellExecutorFixture, erase_range) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_CALL(*mockDriver, eraseSSD(1, 2))
        .Times(1);
    EXPECT_EQ("[Full Erase] Done", shellExecutor.execute("erase_range 1 2", false));
}

TEST_F(ShellExecutorFixture, flush) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_CALL(*mockDriver, flushSSD())
        .Times(1);
    EXPECT_EQ("[Flush] Done", shellExecutor.execute("flush", false));
}

TEST_F(ShellExecutorFixture, script1_fullmatching) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(100)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(100)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("1_FullWriteAndReadCompare", false));
}

TEST_F(ShellExecutorFixture, script1_wildcard) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(100)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(100)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("1_", false));
}

TEST_F(ShellExecutorFixture, script2_fullmatching) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("2_PartialLBAWrite", false));
}

TEST_F(ShellExecutorFixture, script2_wildcard) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(120)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("2_", false));
}

TEST_F(ShellExecutorFixture, script3_fullmatching) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(400)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(400)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("3_WriteReadAging", false));
}

TEST_F(ShellExecutorFixture, script3_wildcard) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    std::unordered_map<int, int> memory;

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(400)
        .WillRepeatedly(::testing::Invoke([&memory](int pos, int data) { memory[pos] = data; }));

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(400)
        .WillRepeatedly(::testing::Invoke([&memory](int pos) -> int {
            auto it = memory.find(pos);
            if (it != memory.end()) {
                return it->second;
            }
            return -1;  // 해당 위치에 데이터가 없을 경우
        }));

    EXPECT_EQ("PASS", shellExecutor.execute("3_", false));
}

TEST_F(ShellExecutorFixture, helpCmd) {
    EXPECT_THAT(shellExecutor.execute("help", false),
        HasSubstr(
        "\n\n COMMAND \n\n"
            "READ command: read [LBA]\n"
            "Write command: write [LBA] [DATA:(ex)0x123456]\n"
            "Full Read command: fullread\n"
            "Full Write command: fullwrite [DATA:(ex)0x123456]\n"
            "Erase command: erase [LBA] [SIZE]\n"
            "Erase Range command: erase_range [start LBA] [end LBA]\n"
            "Flush command: flush\n"
            "Exit command: exit\n"
        "\n\n SCRIPT \n\n"
        ));
}

TEST_F(ShellExecutorFixture, fullReadFullWriteTest1) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);
    unsigned int writeData = 0x123456;
    string hexString = ShellUtil::getUtilObj().toHexFormat(writeData);
    string fullreadResult = "";

    for (int i = 0; i < 100; i++) {
        fullreadResult += "[Full Read] LBA " + ShellUtil::getUtilObj().toTwoDigitString(i) + " " +
                          hexString + " \n";
    }

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, writeData))
        .Times(100)
        .WillRepeatedly(testing::Return());

    EXPECT_CALL(*mockDriver, readSSD(::testing::_))
        .Times(100)
        .WillRepeatedly(testing::Return(writeData));

    EXPECT_EQ("[Full Write] Done", shellExecutor.execute("fullwrite " + hexString, false));
    EXPECT_EQ(fullreadResult, shellExecutor.execute("fullread", false));
}

TEST_F(ShellExecutorFixture, script4_fullmatching) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(2940);

    EXPECT_CALL(*mockDriver, eraseSSD(::testing::_, ::testing::_))
        .Times(1471);

    EXPECT_EQ("PASS", shellExecutor.execute("4_EraseAndWriteAging", false));
}

TEST_F(ShellExecutorFixture, script4_wildcardMatching) {
    ScriptLoader loader;
    loader.loadScript();

    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    // write 호출 시 해당 위치에 데이터를 저장
    EXPECT_CALL(*mockDriver, writeSSD(::testing::_, ::testing::_))
        .Times(2940);

    EXPECT_CALL(*mockDriver, eraseSSD(::testing::_, ::testing::_))
        .Times(1471);

    EXPECT_EQ("PASS", shellExecutor.execute("4_", false));
}