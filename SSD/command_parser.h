#pragma once
#include "FileManager.h"
#define CMD_WRITE (0)
#define CMD_READ (1)
#define CMD_ERASE (2)
#define MAX_LBA_COUNT (100)
#define MAX_ERASE_SIZE (10)
using namespace std;
struct CmdInfo {
	char CMDType;
	string LBA;
	char input_data[20];
};

struct SSDCommand {
	int CMDType = 0;
	int LBA = 0;
	int EraseEndLBA = 0;
	bool IsValid = false;
	char input_data[20] = {};
};
class CMDBuffer {
public:
	// IsFull > Flush(If need) > Append > Merge
	void AppendCMD(SSDCommand InputParam) {
		// append
		cmd_array[empty_buffer_idx] = InputParam;
		cmd_array[empty_buffer_idx++].IsValid = true;
		MergeCMD();
	}
	int CheckValidCmdCount(void) {
		int result = 0;
		for (int nIter = 0; nIter < 5; nIter++) {
			if (cmd_array[nIter].IsValid == true) {
				result++;
			}
		}
		return result;
	}
private:
	void MergeEraseRange(SSDCommand* prevCMD, SSDCommand* curCMD) {
		int EraseCount = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA) - min(curCMD->LBA, prevCMD->LBA) + 1;
		if (EraseCount <= MAX_ERASE_SIZE) {
			curCMD->LBA = min(curCMD->LBA, prevCMD->LBA);
			curCMD->EraseEndLBA = max(curCMD->EraseEndLBA, prevCMD->EraseEndLBA);
			prevCMD->IsValid = false;
		}
	}
	void MergeCMD(void) {
		for (int cmd_offset = 1; cmd_offset < empty_buffer_idx; cmd_offset++) {
			SSDCommand* prevCMD = &cmd_array[empty_buffer_idx - 1 - cmd_offset];
			SSDCommand* curCMD = &cmd_array[empty_buffer_idx - 1];

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
						|| ((prevCMD->LBA < curCMD->LBA) && (prevCMD->EraseEndLBA + 1 >= curCMD->LBA))){
						//mrege
						MergeEraseRange(prevCMD, curCMD);
					}
				}
			}
		}
		int buf_idx = 0;
		SSDCommand output_array[5];
		for (int nIter = 0; nIter < 5; nIter++) {
			if (cmd_array[nIter].IsValid == true) {
				output_array[buf_idx] = cmd_array[nIter];
				buf_idx++;
			}
		}
		for (int nIter = buf_idx; nIter < 5; nIter++) {
			output_array[nIter].IsValid = false;
		}
	}
	SSDCommand cmd_array[5];
	int empty_buffer_idx = 0;
};
class CommandParser {
public:
	CommandParser() : FileObj(std::make_unique<FileManager>()) {
	}

	void PrintError();

	bool checkInvalidity(const char& CMD, string LBAstring, char* data);

	bool parseArg(char CMD, string LBAstring, char* data = NULL);

	int GetCmdType(void) {
		return InputCMD.CMDType;
	}
	int GetLBA(void) {
		return InputCMD.LBA;
	}
	int GetEraseEndLBA(void) {
		return InputCMD.EraseEndLBA;
	}
	char* GetData(void) {
		return InputCMD.input_data;
	}
	SSDCommand GetExcuteCMD(void) {
		return InputCMD;
	}
private:
	SSDCommand InputCMD;
	std::unique_ptr<FileManager> FileObj;
};
