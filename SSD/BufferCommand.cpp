#pragma once
#include "BufferCommand.h"
#include "CommandFileSystem.h"

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

void BufferCommand::doFlush(std::vector<string>& fileList)
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
    }
}
void BufferCommand::DoBufferRead(char* data) {
    MySSD.DoCachedRead(0, data);
}
void BufferCommand::PushCommand(CmdInfo cmdInfo) {
    std::vector<string> fileList = CommandFileMgr->getCmdList();
    if ((cmdInfo.CMDType == CMD_WRITE) || (cmdInfo.CMDType == CMD_ERASE)) {
        // file 이름으로 부터 cmd parsing
        int cmd_idx = 0;
        for (auto file : fileList) {
            string name = file.substr(2);
            if (name == "empty") {// 새로 들어온 cmd는 아직 buffer 에
                string newFileName = to_string(cmd_idx) + "_" +
                    std::string(1, cmdInfo.CMDType) + "_" +
                    cmdInfo.LBAString + "_" +
                    cmdInfo.input_data;
                
                CommandFileMgr->updateFileName(fileList[cmd_idx], newFileName);
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
            doFlush(fileList);
            for (int i = 0; i < 5; i++) {
                string newFileName = to_string(i) + "_" + "empty";

                CommandFileMgr->updateFileName(fileList[i], newFileName);
                fileList[i] = newFileName;
            }
            cmd_idx = 0;
        }
        MergeCMD(cmd_idx, fileList);
    }
    else if (cmdInfo.CMDType == CMD_READ) {
        for (auto file : fileList) {
            string name = file.substr(2);
            if (name == "empty") {
                break;
            }
            else {
                CmdInfo PrevCommand = extractCMDfromFileName(file);
                if ((PrevCommand.CMDType == CMD_WRITE) && (PrevCommand.LBA == cmdInfo.LBA)) {
                    DoBufferRead(PrevCommand.input_data);
                }
                else if ((PrevCommand.CMDType == CMD_ERASE) && (((PrevCommand.LBA) <= (cmdInfo.LBA)) && ((PrevCommand.EraseEndLBA) >= (cmdInfo.LBA)))) {
                    DoBufferRead((char*)"0x00000000");
                }
            }
        }
    }
 
}
