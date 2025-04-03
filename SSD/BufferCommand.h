#pragma once

#include "FileSystem.h"
#include "SSD.cpp"

class BufferCommand {
public:
    BufferCommand(FileSystem& filesystem) : fs(filesystem){
       cmdList.clear();
    }

    CmdInfo extractCMDfromFileName( std::string& file)
    {
        CmdInfo cmd;
        cmd.CMDType = file.at(2);
        cmd.LBA = file.substr(4, 1);
        string temp = file.substr(6);
        strcpy_s(cmd.input_data, temp.c_str());
        return cmd;
    }

    void doFlush(testing::internal::Strings& fileList)
    {
        for (auto file : fileList) {
            CmdInfo input;
            input = extractCMDfromFileName(file);

            ssd.parseArg(input.CMDType, input.LBA, input.input_data);   // validate check
            SSD io;
            io.DoWrite(stoi(input.LBA), input.input_data);
        }
    }

    void updateCmdList(CmdInfo cmdInfo) {
        std::vector<string> fileList = fs.getCmdList();
        std::vector<CmdInfo> cmdList;

        // file 이름으로 부터 cmd parsing
        int idx = 0;
        for (auto file : fileList) {
            string name = file.substr(2);
            if (name == "empty") {// 새로 들어온 cmd는 아직 buffer 에
                string newFileName = to_string(idx) +  "_" +
                    std::string(1, cmdInfo.CMDType) + "_" +
                    cmdInfo.LBA + "_" + 
                    cmdInfo.input_data;

                fs.updateFileName(fileList[idx], newFileName);              
                
                break;
            }     
            else { // 이전에 있던 cmd 들을 list로
                CmdInfo command= extractCMDfromFileName(file);
                cmdList.push_back(command);
                // 새로 들어온 CMD 가 read 였다면, 이전 cmd list 에서 동일 LBA 검색
                if (cmdInfo.LBA == command.LBA)
                {
                    //return cmdInfo.input_data;   data를 return 한다. (how)
                }
            }
            idx++;
        }

        // full
        if (idx == 5) {
            doFlush(fileList);
            for (int i = 0; i < 5; i++) {
                string newFileName = to_string(i) + "_" + "empty";

                fs.updateFileName(fileList[i], newFileName);
            }


        }
    }
private:
    FileSystem& fs;
    std::vector<string> cmdList;
    SSDCommand ssd;
    std::unique_ptr<FileManager> FileMgr;

};

