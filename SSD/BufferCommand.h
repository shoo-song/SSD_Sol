#pragma once

#include "CommandFileSystem.h"
#include "Command_parser.h"
#include "CommandFileSystem.h"
#include "SSD.cpp"

class BufferCommand {
public:
	BufferCommand(CommandFileSystem& filesystem) : CommandFileMgr(std::make_unique<CommandFileSystem>()),
		fs(filesystem) {
	
      cmdList.clear();
    }
    void InitDir(void);
    void InitCmdList(void);
    CmdInfo extractCMDfromFileName( std::string& file);
	int CheckValidCmdCount(void);
    void doFlush(std::vector<string>& fileList);
	void DoBufferRead(char* data);
	void PushCommand(CmdInfo cmdInfo);
   	
private:
    void MergeEraseRange(CmdInfo* prevCMD, CmdInfo* curCMD);
    void MergeCMD(int cmd_index, std::vector<string> fileList);
    std::vector<CmdInfo> cmdList;
    std::unique_ptr<CommandFileSystem> CommandFileMgr;
    std::unique_ptr<DataFileSystem> DataFileMgr;
	SSD MySSD;
};

