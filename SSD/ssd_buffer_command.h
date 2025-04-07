#pragma once

#include "ssd_command_file_system.h"
#include "ssd_command_parser.h"
#include "ssd.cpp"
const int MAX_BUFFER_SIZE = 5;
class BufferCommand {
 public:
  BufferCommand(CommandFileSystem &filesystem) : CommandFileMgr(filesystem) {}
  ~BufferCommand() = default;
  void initDir(void);
  void initCmdList(void);
  int checkValidCmdCount(void);
  void pushCommand(CmdInfo cmdInfo);

  void doWriteAndEraseBuffer(std::vector<std::string> &fileList,
                             CmdInfo &cmdInfo);
  void doFlushBuffer(std::vector<std::string> &fileList);
  void doReadBuffer(CmdInfo &cmdInfo, std::vector<std::string> &fileList);

 private:
  void doFlushOperation(void);
  CmdInfo extractCMDfromFileName(std::string &file);
  void loadPreviousCommand(std::string &file);
  void insertNewCommand(int cmd_idx, CmdInfo &cmdInfo,
                        std::vector<std::string> &fileList);
  int processFileSlot(std::vector<std::string> &fileList, CmdInfo &cmdInfo);

  void handleWriteCommand(CmdInfo *prevCMD, CmdInfo *curCMD);
  void handleEraseCommand(CmdInfo *prevCMD, CmdInfo *curCMD);

  void handleEraseWrite(CmdInfo *curCMD, CmdInfo *prevCMD);
  void handleWriteWrite(CmdInfo *curCMD, CmdInfo *prevCMD);
  void handleWriteErase(CmdInfo *prevCMD, CmdInfo *curCMD);
  void handleEraseErase(CmdInfo *prevCMD, CmdInfo *curCMD);

  string generateFileName(int fileOffset, CmdInfo cmd);
  void updateFileNames(std::vector<std::string> &fileList);
  void changeFileNameToEmpty(std::vector<std::string> &fileList);
  void mergeCMD(int cmd_index, std::vector<string> fileList);
  void mergeEraseRange(CmdInfo *prevCMD, CmdInfo *curCMD);
  bool isEmptyBuffer(string name);

  bool isErasedRange(const CmdInfo &prevCmd, int LBA) const;
  bool isMatchingWrite(const CmdInfo &prevCmd, const CmdInfo &currentCmd) const;

  std::vector<CmdInfo> cmdList;
  CommandFileSystem &CommandFileMgr;
  SSD MySSD;
};

class ICommandHandler {
 public:
  virtual void execute(BufferCommand *self, CmdInfo &cmd,
                       std::vector<std::string> &files) = 0;
  virtual ~ICommandHandler() = default;
};

class WriteOrEraseHandler : public ICommandHandler {
 public:
  void execute(BufferCommand *receiver, CmdInfo &cmd,
               std::vector<std::string> &files) override {
    receiver->doWriteAndEraseBuffer(files, cmd);
  }
};

class ReadHandler : public ICommandHandler {
 public:
  void execute(BufferCommand *receiver, CmdInfo &cmd,
               std::vector<std::string> &files) override {
    receiver->doReadBuffer(cmd, files);
  }
};

class FlushHandler : public ICommandHandler {
 public:
  void execute(BufferCommand *receiver, CmdInfo &cmd,
               std::vector<std::string> &files) override {
    receiver->doFlushBuffer(files);
  }
};
