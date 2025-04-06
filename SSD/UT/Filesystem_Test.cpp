#pragma once
#include <gmock/gmock.h>
#include <sys/stat.h>  // mkdir(), stat()

#include <cstdio>   // remove()
#include <cstdlib>  // system()
#include <memory>   // std::unique_ptr
#include <string>

#include "../CommandExecutor.h"
#include "iostream"

using namespace testing;
// #define UNIT_TEST
class FilesystemFixture : public Test {
 public:
  CommandFileSystem fs;
};

TEST_F(FilesystemFixture, CreateBufferFolder) {
  std::string testDir = "buffer";
  // 1️. 폴더 생성
  fs.createDirectory();
  EXPECT_TRUE(fs.directoryExists(testDir)) << "폴더 생성 실패";

  // 2️. 폴더 삭제
  fs.removeDirectory(testDir);
  EXPECT_FALSE(fs.directoryExists(testDir)) << "폴더 삭제 실패";
}

TEST_F(FilesystemFixture, CreateEmptyFiles) {
  // given : create buffer directory
  fs.createDirectory();

  std::string testFile = "empty";
  EXPECT_FALSE(fs.fileExists(testFile)) << "empty.txt 파일이 존재하지 않음";

  fs.createFiles();

  EXPECT_EQ(true, fs.fileExists("0_empty"));
  EXPECT_EQ(true, fs.fileExists("1_empty"));
  EXPECT_EQ(true, fs.fileExists("2_empty"));
  EXPECT_EQ(true, fs.fileExists("3_empty"));
  EXPECT_EQ(true, fs.fileExists("4_empty"));
}

TEST_F(FilesystemFixture, MakeCmdListFromBufferFiles) {
  // given : initialize output file
  fs.createFiles();

  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  std::vector<std::string> expectedFiles = {"0_empty", "1_empty", "2_empty",
                                            "3_empty", "4_empty"};
  for (auto name : fileNames) {
    EXPECT_THAT(name, AnyOfArray(expectedFiles));
  }
}

TEST_F(FilesystemFixture, updateFileName) {
  // given : initialize output file
  fs.createFiles();

  std::vector<std::string> fileNames;
  fileNames = fs.makeCmdList();

  string newName = "0_W_0_0x12345678";
  string oldName = fileNames[0];
  fs.updateFileName(oldName, newName);
  EXPECT_EQ(true, fs.fileExists(newName));
}