#pragma once
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
void BufferCommand::doReadBuffer(CmdInfo& cmdInfo, std::vector<std::string>& fileList)
{
    MySSD.DoRead(cmdInfo.LBA);
    for (auto file : fileList) {
        string name = file.substr(2);
        if (isEmptyBuffer(name)) {
            break;
        }
        else {
            CmdInfo PrevCommand = extractCMDfromFileName(file);
            if ((PrevCommand.CMDType == CMD_WRITE) && (PrevCommand.LBA == cmdInfo.LBA)) {
                MySSD.DoCachedRead(0, PrevCommand.input_data);
            }
            else if ((PrevCommand.CMDType == CMD_ERASE) && (((PrevCommand.LBA) <= (cmdInfo.LBA)) && ((PrevCommand.EraseEndLBA) >= (cmdInfo.LBA)))) {
                MySSD.DoCachedRead(0, (char*)"0x00000000");
            }
        }
    }
}
void BufferCommand::doWriteAndEraseBuffer(std::vector<std::string>& fileList, CmdInfo& cmdInfo)
{
    // file 이름으로 부터 cmd parsing
    int cmd_idx = 0;
    for (auto file : fileList) {
        string name = file.substr(2);
        if (isEmptyBuffer(name)) {// 새로 들어온 cmd는 아직 buffer 에
            string newFileName = to_string(cmd_idx) + "_" +
                std::string(1, cmdInfo.CMDType) + "_" +
                cmdInfo.LBAString + "_" +
                cmdInfo.input_data;

            CommandFileMgr.updateFileName(fileList[cmd_idx], newFileName);
            fileList[cmd_idx] = newFileName;
            cmdList.push_back(cmdInfo);
            break;
        }
        else { // 이전에 있던 cmd 들을 list로
            CmdInfo command = extractCMDfromFileName(file);
            cmdList.push_back(command);
        }
        cmd_idx++;
    }
    // full
    if (cmd_idx == 5) {
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
void BufferCommand::MergeCMD(int cmd_index, std::vector<string> fileList) {
    for (int cmd_offset = cmd_index - 1; cmd_offset >= 0; cmd_offset--) {
        CmdInfo* prevCMD = &cmdList[cmd_offset];
        CmdInfo* curCMD = &cmdList[cmd_index];
        //check merge
        if (curCMD->CMDType == CMD_WRITE) {
            if (prevCMD->CMDType == CMD_WRITE) {
                // W-W
                if (curCMD->LBA == prevCMD->LBA) {
                    prevCMD->IsValid = false;
                }
            }
            else {
                // E-W
                if (curCMD->LBA == prevCMD->EraseEndLBA) {
                    prevCMD->EraseEndLBA--;
                    if (prevCMD->EraseEndLBA < prevCMD->LBA) {
                        prevCMD->IsValid = false;
                    }
                }
                else if (curCMD->LBA == prevCMD->LBA) {
                    prevCMD->LBA++;
                    if (prevCMD->LBA > prevCMD->EraseEndLBA) {
                        prevCMD->IsValid = false;
                    }
                }
            }
        }
        else if (curCMD->CMDType == CMD_ERASE) {
            if (prevCMD->CMDType == CMD_WRITE) {
                // W-E
                if (((prevCMD->LBA) >= (curCMD->LBA)) && ((prevCMD->LBA) <= (curCMD->EraseEndLBA))) {
                    prevCMD->IsValid = false;
                }
            }
            else {
                // E-E
                if (((prevCMD->LBA == curCMD->LBA) || (prevCMD->EraseEndLBA == curCMD->EraseEndLBA))
                    || ((prevCMD->LBA > curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA + 1))
                    || ((prevCMD->LBA < curCMD->LBA) && (prevCMD->EraseEndLBA + 1 >= curCMD->LBA))) {
                    //mrege
                    MergeEraseRange(prevCMD, curCMD);
                }
            }
        }
    }
    int fileoffset = 0;
    for (auto cmd : cmdList) {
        if (cmd.IsValid == true) {
            string newFileName = to_string(fileoffset) + "_" +
                std::string(1, cmd.CMDType) + "_" +
                cmd.LBAString + "_" +
                cmd.input_data;
            CommandFileMgr.updateFileName(fileList[fileoffset++], newFileName);
        }
    }
    for (int nIter = fileoffset; nIter < 5; nIter++) {
        string newFileName = to_string(nIter) + "_" + "empty";
        CommandFileMgr.updateFileName(fileList[nIter], newFileName);
    }
}


void BufferCommand::PushCommand(CmdInfo cmdInfo) {
    std::vector<string> fileList = CommandFileMgr.getCmdList();
    if ((cmdInfo.CMDType == CMD_WRITE) || (cmdInfo.CMDType == CMD_ERASE)) {
        doWriteAndEraseBuffer(fileList, cmdInfo);
    }
    else if (cmdInfo.CMDType == CMD_READ) {
        doReadBuffer(cmdInfo, fileList);
    }
    else if (cmdInfo.CMDType == CMD_FLUSH) {
        doFlushBuffer(fileList);
    }

}