#pragma once

#include "CommandFileSystem.h"
#include "Command_parser.h"
#include "CommandFileSystem.h"
#include "SSD.cpp"

class BufferCommand {
public:
	BufferCommand(CommandFileSystem& filesystem) : CommandFileMgr(filesystem) {     
      cmdList.clear();
    }
    void InitDir(void);
    void InitCmdList(void);
    CmdInfo extractCMDfromFileName( std::string& file);
	int CheckValidCmdCount(void);
    void doFlushOperation(void);
	void PushCommand(CmdInfo cmdInfo);

    void doFlushBuffer(std::vector<std::string>& fileList);

    void doReadBuffer(CmdInfo& cmdInfo, std::vector<std::string>& fileList);

    void doWriteAndEraseBuffer(std::vector<std::string>& fileList, CmdInfo& cmdInfo);

    void changeFileNameToEmpty( std::vector<std::string>& fileList);
   	
private:
    void MergeEraseRange(CmdInfo* prevCMD, CmdInfo* curCMD);
    void MergeCMD(int cmd_index, std::vector<string> fileList);
    std::vector<CmdInfo> cmdList;
    CommandFileSystem& CommandFileMgr;
	SSD MySSD;

    bool isEmptyBuffer(string name) {
        return (name == "empty") ? true : false;
    }

};

