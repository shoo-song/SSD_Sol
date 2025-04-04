#pragma once

#include "FileSystem.h"
#include "Command_parser.h"
#include "SSD.cpp"

class BufferCommand {
public:
    BufferCommand(FileSystem& filesystem) : fs(filesystem){
      cmdList.clear();
    }

    CmdInfo extractCMDfromFileName( std::string& file);

    void doFlush(std::vector<string>& fileList);

    void updateCmdList(CmdInfo cmdInfo);
private:
    FileSystem& fs;
    std::vector<CmdInfo> cmdList;
    CommandParser ssd;
    std::unique_ptr<FileManager> FileMgr;

};

