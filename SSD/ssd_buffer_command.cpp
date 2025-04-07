#pragma once
#include "ssd_buffer_command.h"
#include "ssd_command_parser.h"

#include <unordered_map>

#include "ssd_command_file_system.h"
void BufferCommand::initDir(void) { CommandFileMgr.removeDirectory("buffer"); }
void BufferCommand::initCmdList(void) { cmdList.clear(); }
CmdInfo BufferCommand::extractCMDfromFileName(std::string &file) {
  CmdInfo cmd;
  cmd.CMDType = file.at(2);
  cmd.LBAString = file.substr(4, 2);
  cmd.LBA = stoi(cmd.LBAString);
  cmd.IsValid = true;
  string temp = file.substr(7);
  strcpy_s(cmd.input_data, temp.c_str());
  if (cmd.CMDType == CMD_ERASE) {
      cmd.EraseEndLBA = cmd.LBA + stoi(cmd.input_data) - 1;
  }
  return cmd;
}
int BufferCommand::checkValidCmdCount(void) {
  int result = 0;
  for (auto cmd : cmdList) {
    if (cmd.IsValid == true) result++;
  }
  return result;
}
void BufferCommand::doFlushOperation(void) {
  for (auto CurCmd : cmdList) {
    if (CurCmd.CMDType == CMD_WRITE) {
      MySSD.doWrite(CurCmd.LBA, CurCmd.input_data);
    } else if (CurCmd.CMDType == CMD_ERASE) {
      MySSD.doErase(CurCmd.LBA, stoi(CurCmd.input_data));
    } else {
      // error
    }
    CurCmd.IsValid = false;
  }
}

void BufferCommand::doFlushBuffer(std::vector<std::string> &fileList) {
  for (auto file : fileList) {
    string name = file.substr(2);
    if (!isEmptyBuffer(name)) {
      CmdInfo command = extractCMDfromFileName(file);
      cmdList.push_back(command);
    }
  }
  doFlushOperation();

  changeFileNameToEmpty(fileList);
}

bool BufferCommand::isMatchingWrite(const CmdInfo &prevCmd,
                                    const CmdInfo &currentCmd) const {
  return prevCmd.CMDType == CMD_WRITE && prevCmd.LBA == currentCmd.LBA;
}

bool BufferCommand::isErasedRange(const CmdInfo &prevCmd, int LBA) const {
  return prevCmd.CMDType == CMD_ERASE && prevCmd.LBA <= LBA &&
         prevCmd.EraseEndLBA >= LBA;
}
void BufferCommand::doReadBuffer(CmdInfo &cmdInfo,
                                 std::vector<std::string> &fileList) {
  MySSD.doRead(cmdInfo.LBA);
  for (auto file : fileList) {
    string name = file.substr(2);
    if (isEmptyBuffer(name)) {
      break;
    }
    CmdInfo prevCmd = extractCMDfromFileName(file);
    if (isMatchingWrite(prevCmd, cmdInfo)) {
      MySSD.doCacheRead(0, prevCmd.input_data);
    } else if (isErasedRange(prevCmd, cmdInfo.LBA)) {
      static char erasedData[] = "0x00000000";
      MySSD.doCacheRead(0, erasedData);
    }
  }
}

void BufferCommand::loadPreviousCommand(std::string &file) {
  CmdInfo command = extractCMDfromFileName(file);
  cmdList.push_back(command);
}
void BufferCommand::insertNewCommand(int cmd_idx, CmdInfo &cmdInfo,
                                     std::vector<std::string> &fileList) {
  string newFileName = generateFileName(cmd_idx, cmdInfo);

  CommandFileMgr.updateFileName(fileList[cmd_idx], newFileName);
  fileList[cmd_idx] = newFileName;
  cmdList.push_back(cmdInfo);
}

int BufferCommand::processFileSlot(std::vector<std::string> &fileList,
                                   CmdInfo &cmdInfo) {
  // for (auto file : fileList) {
  for (int cmd_idx = 0; cmd_idx < MAX_BUFFER_SIZE; ++cmd_idx) {
    std::string &file = fileList[cmd_idx];
    string name = file.substr(2);
    if (isEmptyBuffer(name)) {  // 새로 들어온 cmd는 아직 buffer 에

      insertNewCommand(cmd_idx, cmdInfo, fileList);
      return cmd_idx;
    } else {  // 이전에 있던 cmd 들을 list로
      loadPreviousCommand(file);
    }
  }
  return MAX_BUFFER_SIZE;
}
void BufferCommand::doWriteAndEraseBuffer(std::vector<std::string> &fileList,
                                          CmdInfo &cmdInfo) {
  // file 이름으로 부터 cmd parsing
  int cmd_idx = processFileSlot(fileList, cmdInfo);

  // full
  if (cmd_idx == MAX_BUFFER_SIZE) {
    doFlushOperation();
    changeFileNameToEmpty(fileList);

    cmdList[0] = cmdInfo;
    cmd_idx = 0;
  }
  mergeCMD(cmd_idx, fileList);
}

void BufferCommand::changeFileNameToEmpty(std::vector<std::string> &fileList) {
  int idx = 0;
  for (auto cmd : cmdList) {
    string newFileName = to_string(idx) + "_" + "empty";
    CommandFileMgr.updateFileName(fileList[idx], newFileName);
    fileList[idx] = newFileName;
    cmdList[idx].IsValid = false;
    idx++;
  }
}
void BufferCommand::mergeEraseRange(CmdInfo *prevCMD, CmdInfo *curCMD) {
  int EraseCount = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA) -
                   min(curCMD->LBA, prevCMD->LBA) + 1;
  if (EraseCount <= MAX_ERASE_SIZE) {
      curCMD->LBA = min(curCMD->LBA, prevCMD->LBA);
      curCMD->LBAString = to_string(curCMD->LBA);
      curCMD->EraseEndLBA = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA);
      strcpy_s(curCMD->input_data, to_string(EraseCount).data());
      prevCMD->IsValid = false;
  }
}

void BufferCommand::handleEraseWrite(CmdInfo *curCMD, CmdInfo *prevCMD) {
    // 이전 상태 저장
    int originalLBA = prevCMD->LBA;
    int originalEraseEnd = prevCMD->EraseEndLBA;

    // 연산 결과를 조건 없이 계산
    int eraseBegin = originalLBA + (curCMD->LBA == originalLBA);
    int eraseEnd = originalEraseEnd - (curCMD->LBA == originalEraseEnd);

    // 적용
    prevCMD->LBA = eraseBegin;
    prevCMD->EraseEndLBA = eraseEnd;
    strcpy_s(prevCMD->input_data, to_string(eraseEnd - eraseBegin + 1).c_str());
    prevCMD->LBAString = to_string(eraseBegin);
    // 유효성 검사
    prevCMD->IsValid = eraseBegin <= eraseEnd;
}

void BufferCommand::handleWriteWrite(CmdInfo *curCMD, CmdInfo *prevCMD) {
  prevCMD->IsValid = (curCMD->LBA != prevCMD->LBA);
}
void BufferCommand::handleWriteCommand(CmdInfo *prevCMD, CmdInfo *curCMD) {
  if (prevCMD->CMDType == CMD_WRITE) {
    // W-W
    handleWriteWrite(curCMD, prevCMD);
  } else {
    // E-W
    handleEraseWrite(curCMD, prevCMD);
  }
}
void BufferCommand::handleWriteErase(CmdInfo *prevCMD, CmdInfo *curCMD) {
  bool isOverlapping =
      (prevCMD->LBA >= curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA);
  prevCMD->IsValid = !isOverlapping;
}
void BufferCommand::handleEraseErase(CmdInfo *prevCMD, CmdInfo *curCMD) {
  bool sameStart = (prevCMD->LBA == curCMD->LBA);
  bool sameEnd = (prevCMD->EraseEndLBA == curCMD->EraseEndLBA);
  bool prevStartInsideCur =
      (prevCMD->LBA > curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA + 1);
  bool prevEndInsideCur =
      (prevCMD->LBA < curCMD->LBA) && (prevCMD->EraseEndLBA + 1 >= curCMD->LBA);

  if (sameStart || sameEnd || prevStartInsideCur || prevEndInsideCur) {
    // mrege
    mergeEraseRange(prevCMD, curCMD);
  }
}
void BufferCommand::handleEraseCommand(CmdInfo *prevCMD, CmdInfo *curCMD) {
  if (prevCMD->CMDType == CMD_WRITE) {
    // W-E
    handleWriteErase(prevCMD, curCMD);
  } else {
    // E-E
    handleEraseErase(prevCMD, curCMD);
  }
}
string BufferCommand::generateFileName(int fileOffset, CmdInfo cmd) {
    CommandParser parser;
    string LBAString = parser.toTwoDigitString(stoi(cmd.LBAString));
  return to_string(fileOffset) + "_" + std::string(1, cmd.CMDType) + "_" +
      LBAString + "_" + cmd.input_data;
}

bool BufferCommand::isEmptyBuffer(string name) {
  return (name == "empty") ? true : false;
}

void BufferCommand::updateFileNames(std::vector<std::string> &fileList) {
  int fileoffset = 0;
  for (auto cmd : cmdList) {
    if (!cmd.IsValid) continue;

    string newFileName = generateFileName(fileoffset, cmd);
    CommandFileMgr.updateFileName(fileList[fileoffset], newFileName);
    ++fileoffset;
  }
  for (int nIter = fileoffset; nIter < 5; nIter++) {
    string newFileName = to_string(nIter) + "_" + "empty";
    CommandFileMgr.updateFileName(fileList[nIter], newFileName);
  }
}
void BufferCommand::mergeCMD(int cmd_index, std::vector<string> fileList) {
  for (int cmd_offset = cmd_index - 1; cmd_offset >= 0; cmd_offset--) {
    CmdInfo *prevCMD = &cmdList[cmd_offset];
    CmdInfo *curCMD = &cmdList[cmd_index];
    // check merge
    if (curCMD->CMDType == CMD_WRITE) {
      handleWriteCommand(prevCMD, curCMD);
    } else if (curCMD->CMDType == CMD_ERASE) {
      handleEraseCommand(prevCMD, curCMD);
    }
  }
  updateFileNames(fileList);
}

void BufferCommand::pushCommand(CmdInfo cmdInfo) {
  std::vector<std::string> fileList = CommandFileMgr.getCmdList();

  static const std::unordered_map<char, std::shared_ptr<ICommandHandler>>
      handlers = {
          {CMD_WRITE, std::make_shared<WriteOrEraseHandler>()},
          {CMD_ERASE, std::make_shared<WriteOrEraseHandler>()},
          {CMD_READ, std::make_shared<ReadHandler>()},
          {CMD_FLUSH, std::make_shared<FlushHandler>()},
      };

  auto it = handlers.find(cmdInfo.CMDType);
  if (it != handlers.end()) {
    it->second->execute(this, cmdInfo, fileList);
  }
}