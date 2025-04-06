#pragma once
#include <unordered_map>
#include "BufferCommand.h"
#include "CommandFileSystem.h"
void BufferCommand::InitDir(void) {
    CommandFileMgr.removeDirectory("buffer");
}
void BufferCommand::InitCmdList(void) {
    cmdList.clear();
}
CmdInfo BufferCommand::extractCMDfromFileName(std::string& file)
{
    CmdInfo cmd;
    cmd.CMDType = file.at(2);
    cmd.LBAString = file.substr(4, 2);
    cmd.LBA = stoi(cmd.LBAString);
    cmd.IsValid = true;
    string temp = file.substr(7);
    strcpy_s(cmd.input_data, temp.c_str());
    cmd.EraseEndLBA = cmd.LBA + stoi(cmd.input_data) - 1;
    return cmd;
}
int BufferCommand::CheckValidCmdCount(void) {
    int result = 0;
    for (auto cmd : cmdList) {
        if (cmd.IsValid == true) result++;
    }
    return result;
}
void BufferCommand::doFlushOperation(void)
{
    for (auto CurCmd : cmdList) {
        if (CurCmd.CMDType == CMD_WRITE) {
            MySSD.DoWrite(CurCmd.LBA, CurCmd.input_data);
        }
        else if (CurCmd.CMDType == CMD_ERASE) {
            MySSD.DoErase(CurCmd.LBA, stoi(CurCmd.input_data));
        }
        else {
            //error
        }
        CurCmd.IsValid = false;
    }
}

void BufferCommand::doFlushBuffer(std::vector<std::string>& fileList)
{
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

bool BufferCommand::isMatchingWrite(const CmdInfo& prevCmd, const CmdInfo& currentCmd) const {
    return prevCmd.CMDType == CMD_WRITE && prevCmd.LBA == currentCmd.LBA;
}

bool BufferCommand::isErasedRange(const CmdInfo& prevCmd, int LBA) const {
    return prevCmd.CMDType == CMD_ERASE &&
        prevCmd.LBA <= LBA && prevCmd.EraseEndLBA >= LBA;
}
void BufferCommand::doReadBuffer(CmdInfo& cmdInfo, std::vector<std::string>& fileList)
{
    MySSD.DoRead(cmdInfo.LBA);
    for (auto file : fileList) {
        string name = file.substr(2);
        if (isEmptyBuffer(name)) {
            break;
        }
        CmdInfo prevCmd = extractCMDfromFileName(file);
        if (isMatchingWrite(prevCmd, cmdInfo)) {
            MySSD.DoCachedRead(0, prevCmd.input_data);
        }
        else if (isErasedRange(prevCmd, cmdInfo.LBA)) {
            static char erasedData[] = "0x00000000";
            MySSD.DoCachedRead(0, erasedData);
        }
    }
}

void BufferCommand::loadPreviousCommand(std::string& file)
{
    CmdInfo command = extractCMDfromFileName(file);
    cmdList.push_back(command);
}
void BufferCommand::insertNewCommand(int cmd_idx, CmdInfo& cmdInfo, std::vector<std::string>& fileList)
{
    string newFileName = generateFileName(cmd_idx, cmdInfo);

    CommandFileMgr.updateFileName(fileList[cmd_idx], newFileName);
    fileList[cmd_idx] = newFileName;
    cmdList.push_back(cmdInfo);
}

int BufferCommand::processFileSlot(std::vector<std::string>& fileList,CmdInfo& cmdInfo)
{
    //for (auto file : fileList) {
    for (int cmd_idx = 0; cmd_idx < MAX_BUFFER_SIZE; ++cmd_idx) {
        std::string& file = fileList[cmd_idx];
        string name = file.substr(2);
        if (isEmptyBuffer(name)) {// 새로 들어온 cmd는 아직 buffer 에

            insertNewCommand(cmd_idx, cmdInfo, fileList);
            return cmd_idx;
        }
        else { // 이전에 있던 cmd 들을 list로
            loadPreviousCommand(file);
        }        
    }
    return MAX_BUFFER_SIZE;
}
void BufferCommand::doWriteAndEraseBuffer(std::vector<std::string>& fileList, CmdInfo& cmdInfo)
{
    // file 이름으로 부터 cmd parsing
    int cmd_idx = processFileSlot(fileList, cmdInfo);

    // full
    if (cmd_idx == MAX_BUFFER_SIZE) {
        doFlushOperation();
        changeFileNameToEmpty(fileList);

        cmdList[0] = cmdInfo;
        cmd_idx = 0;

    }
    MergeCMD(cmd_idx, fileList);
}


void BufferCommand::changeFileNameToEmpty( std::vector<std::string>& fileList)
{
    int idx = 0;
    for (auto cmd : cmdList) {
        string newFileName = to_string(idx) + "_" + "empty";
        CommandFileMgr.updateFileName(fileList[idx], newFileName);
        fileList[idx] = newFileName;
        cmdList[idx].IsValid = false;
        idx++;
    }
}
void BufferCommand::MergeEraseRange(CmdInfo* prevCMD, CmdInfo* curCMD) {
    int EraseCount = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA) - min(curCMD->LBA, prevCMD->LBA) + 1;
    if (EraseCount <= MAX_ERASE_SIZE) {
        curCMD->LBA = min(curCMD->LBA, prevCMD->LBA);
        curCMD->LBAString = to_string(curCMD->LBA);
        curCMD->EraseEndLBA = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA);
        strcpy_s(curCMD->input_data, to_string(EraseCount).data());
        prevCMD->IsValid = false;
    }
}

void BufferCommand::HandleEraseWrite(CmdInfo* curCMD, CmdInfo* prevCMD)
{
    // 이전 상태 저장
    int originalLBA = prevCMD->LBA;
    int originalEraseEnd = prevCMD->EraseEndLBA;

    // 연산 결과를 조건 없이 계산
    int eraseBegin = originalLBA + (curCMD->LBA == originalLBA);
    int eraseEnd = originalEraseEnd - (curCMD->LBA == originalEraseEnd);

    // 적용
    prevCMD->LBA = eraseBegin;
    prevCMD->EraseEndLBA = eraseEnd;

    // 유효성 검사
    prevCMD->IsValid = eraseBegin <= eraseEnd;
}

void BufferCommand::HandleWriteWrite(CmdInfo* curCMD, CmdInfo* prevCMD)
{
    prevCMD->IsValid = (curCMD->LBA != prevCMD->LBA);
}
void BufferCommand::HandleWriteCommand(CmdInfo* prevCMD, CmdInfo* curCMD)
{
    if (prevCMD->CMDType == CMD_WRITE) {
        // W-W
        HandleWriteWrite(curCMD, prevCMD);
    }
    else {
        // E-W
        HandleEraseWrite(curCMD, prevCMD);
    }
}
void BufferCommand::HandleWriteErase(CmdInfo* prevCMD, CmdInfo* curCMD)
{
    bool isOverlapping = (prevCMD->LBA >= curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA);
    prevCMD->IsValid = !isOverlapping;
}
void BufferCommand::HandleEraseErase(CmdInfo* prevCMD, CmdInfo* curCMD)
{
    bool sameStart = (prevCMD->LBA == curCMD->LBA);
    bool sameEnd = (prevCMD->EraseEndLBA == curCMD->EraseEndLBA);
    bool prevStartInsideCur = (prevCMD->LBA > curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA + 1);
    bool prevEndInsideCur = (prevCMD->LBA < curCMD->LBA) && (prevCMD->EraseEndLBA + 1 >= curCMD->LBA);

    if (sameStart || sameEnd || prevStartInsideCur|| prevEndInsideCur) {
        //mrege
        MergeEraseRange(prevCMD, curCMD);
    }
}
void BufferCommand::HandleEraseCommand(CmdInfo* prevCMD, CmdInfo* curCMD)
{
    if (prevCMD->CMDType == CMD_WRITE) {
        // W-E
        HandleWriteErase(prevCMD, curCMD);
    }
    else {
        // E-E
        HandleEraseErase(prevCMD, curCMD);
    }
}
string BufferCommand::generateFileName(int fileOffset, CmdInfo cmd) {
    return to_string(fileOffset) + "_" +
        std::string(1, cmd.CMDType) + "_" +
        cmd.LBAString + "_" +
        cmd.input_data;
}

bool BufferCommand::isEmptyBuffer(string name) {
    return (name == "empty") ? true : false;
}

void BufferCommand::UpdateFileNames(std::vector<std::string>& fileList)
{
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
void BufferCommand::MergeCMD(int cmd_index, std::vector<string> fileList) {
    for (int cmd_offset = cmd_index - 1; cmd_offset >= 0; cmd_offset--) {
        CmdInfo* prevCMD = &cmdList[cmd_offset];
        CmdInfo* curCMD = &cmdList[cmd_index];
        //check merge
        if (curCMD->CMDType == CMD_WRITE) {
            HandleWriteCommand(prevCMD, curCMD);
        }
        else if (curCMD->CMDType == CMD_ERASE) {
            HandleEraseCommand(prevCMD, curCMD);
        }
    }
    UpdateFileNames(fileList);
}

void BufferCommand::PushCommand(CmdInfo cmdInfo) {
    std::vector<std::string> fileList = CommandFileMgr.getCmdList();

    static const std::unordered_map<char, std::shared_ptr<ICommandHandler>> handlers = {
        { CMD_WRITE, std::make_shared<WriteOrEraseHandler>() },
        { CMD_ERASE, std::make_shared<WriteOrEraseHandler>() },
        { CMD_READ,  std::make_shared<ReadHandler>() },
        { CMD_FLUSH, std::make_shared<FlushHandler>() },
    };

    auto it = handlers.find(cmdInfo.CMDType);
    if (it != handlers.end()) {
        it->second->execute(this, cmdInfo, fileList);
    }
}