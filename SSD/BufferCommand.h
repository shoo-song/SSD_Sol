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

    CmdInfo extractCMDfromFileName( std::string& file);

    void doFlush(std::vector<string>& fileList);
	void DoBufferRead(char* data);
	void PushCommand(CmdInfo cmdInfo);
   	
private:
	void MergeEraseRange(CmdInfo* prevCMD, CmdInfo* curCMD) {
		int EraseCount = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA) - min(curCMD->LBA, prevCMD->LBA) + 1;
		if (EraseCount <= MAX_ERASE_SIZE) {
			curCMD->LBA = min(curCMD->LBA, prevCMD->LBA);
			curCMD->EraseEndLBA = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA);
			prevCMD->IsValid = false;
		}
	}
	void MergeCMD(int cmd_index, std::vector<string> fileList) {
		for (int cmd_offset = cmd_index-1; cmd_offset >= 0; cmd_offset--) {
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
				CommandFileMgr->updateFileName(fileList[fileoffset++], newFileName);
			}
		}
		for (int nIter = fileoffset; nIter < 5; nIter++) {
			string newFileName = to_string(nIter) + "_" + "empty";
			CommandFileMgr->updateFileName(fileList[nIter], newFileName);
		}
	}
	CommandFileSystem& fs;
    std::vector<CmdInfo> cmdList;
    std::unique_ptr<CommandFileSystem> CommandFileMgr;
    std::unique_ptr<DataFileSystem> DataFileMgr;
	SSD MySSD;
};

