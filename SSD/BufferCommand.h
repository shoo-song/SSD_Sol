#pragma once

#include "CommandFileSystem.h"
#include "Command_parser.h"
#include "CommandFileSystem.h"
#include "SSD.cpp"
const int MAX_BUFFER_SIZE = 5;
class BufferCommand {
public:
	BufferCommand(CommandFileSystem& filesystem) : CommandFileMgr(filesystem) {          
    }
    ~BufferCommand() = default;
    void InitDir(void);
    void InitCmdList(void);   
	int CheckValidCmdCount(void);    
	void PushCommand(CmdInfo cmdInfo);

    void doWriteAndEraseBuffer(std::vector<std::string>& fileList, CmdInfo& cmdInfo);
    void doFlushBuffer(std::vector<std::string>& fileList);
    void doReadBuffer(CmdInfo& cmdInfo, std::vector<std::string>& fileList);
private:    
    void doFlushOperation(void);    
    CmdInfo extractCMDfromFileName(std::string& file);
    void loadPreviousCommand(std::string& file);
    void insertNewCommand(int cmd_idx, CmdInfo& cmdInfo, std::vector<std::string>& fileList);
    int processFileSlot(std::vector<std::string>& fileList, CmdInfo& cmdInfo);
    
    void HandleWriteCommand(CmdInfo* prevCMD, CmdInfo* curCMD);
    void HandleEraseCommand(CmdInfo* prevCMD, CmdInfo* curCMD);

    void HandleEraseWrite(CmdInfo* curCMD, CmdInfo* prevCMD);
    void HandleWriteWrite(CmdInfo* curCMD, CmdInfo* prevCMD);    
    void HandleWriteErase(CmdInfo* prevCMD, CmdInfo* curCMD);
    void HandleEraseErase(CmdInfo* prevCMD, CmdInfo* curCMD);
    
    string generateFileName(int fileOffset, CmdInfo cmd);
    void UpdateFileNames(std::vector<std::string>& fileList);
    void changeFileNameToEmpty(std::vector<std::string>& fileList);
    void MergeCMD(int cmd_index, std::vector<string> fileList);
    void MergeEraseRange(CmdInfo* prevCMD, CmdInfo* curCMD);    
    bool isEmptyBuffer(string name);

    bool isErasedRange(const CmdInfo& prevCmd, int LBA) const;
    bool isMatchingWrite(const CmdInfo& prevCmd, const CmdInfo& currentCmd) const;

    std::vector<CmdInfo> cmdList;
    CommandFileSystem& CommandFileMgr;
	SSD MySSD;    
};


class ICommandHandler {
public:
    virtual void execute(BufferCommand* self, CmdInfo& cmd, std::vector<std::string>& files) = 0;
    virtual ~ICommandHandler() = default;
};

class WriteOrEraseHandler : public ICommandHandler {
public:
    void execute(BufferCommand* receiver, CmdInfo& cmd, std::vector<std::string>& files) override {
        receiver->doWriteAndEraseBuffer(files, cmd);
    }
};

class ReadHandler : public ICommandHandler {
public:
    void execute(BufferCommand* receiver, CmdInfo& cmd, std::vector<std::string>& files) override {
        receiver->doReadBuffer(cmd, files);
    }
};

class FlushHandler : public ICommandHandler {
public:
    void execute(BufferCommand* receiver, CmdInfo& cmd, std::vector<std::string>& files) override {
        receiver->doFlushBuffer(files);
    }
};

