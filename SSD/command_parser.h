#pragma once
#include "FileManager.h"
#define CMD_WRITE (0)
#define CMD_READ (1)
#define CMD_ERASE (2)
#define MAX_LBA (99)
#define MAX_ERASE_SIZE (10)
struct SSDCommand {
	int CMDType = 0;
	int LBA = 0;
	int EraseEndLBA = 0;
	bool IsInvalid = 1;
	char input_data[20] = {};
};
class CMDBuffer {
public:
	// IsFull > Flush(If need) > Append > Merge
	void AppendCMD(SSDCommand InputParam) {
		// append
		cmd_array[empty_buffer_idx] = InputParam;
		cmd_array[empty_buffer_idx++].IsInvalid = false;
		MergeCMD();
	}
	int CheckValidCmdCount(void) {
		int result = 0;
		for (int nIter = 0; nIter < 5; nIter++) {
			if (cmd_array[nIter].IsInvalid == false) {
				result++;
			}
		}
		return result;
	}
private:
	void MergeCMD(void) {
		for (int cmd_offset = 1; cmd_offset < empty_buffer_idx; cmd_offset++) {
			SSDCommand* prevCMD = &cmd_array[empty_buffer_idx - 1 - cmd_offset];
			SSDCommand* curCMD = &cmd_array[empty_buffer_idx - 1];

			//check merge
			if (curCMD->CMDType == CMD_WRITE) {
				if (prevCMD->CMDType == CMD_WRITE) {
					// W-W
					if (curCMD->LBA == prevCMD->LBA) {
						prevCMD->IsInvalid = true;
					}
				}
				else {
					// E-W
					if (curCMD->LBA == prevCMD->EraseEndLBA) {
						prevCMD->EraseEndLBA--;
						if (prevCMD->EraseEndLBA < prevCMD->LBA) {
							prevCMD->IsInvalid = true;
						}
					}
					else if (curCMD->LBA == prevCMD->LBA) {
						prevCMD->LBA++;
						if (prevCMD->LBA > prevCMD->EraseEndLBA) {
							prevCMD->IsInvalid = true;
						}
					}
				}
			}
			else if (curCMD->CMDType == CMD_ERASE) {
				if (prevCMD->CMDType == CMD_WRITE) {
					// W-E
					if (((prevCMD->LBA) >= (curCMD->LBA)) && ((prevCMD->LBA) <= (curCMD->EraseEndLBA))) {
						prevCMD->IsInvalid = true;
					}
				}
				else {
					// E-E
					if ((prevCMD->LBA > curCMD->LBA) && (prevCMD->LBA <= curCMD->EraseEndLBA)) {
						curCMD->EraseEndLBA = prevCMD->EraseEndLBA;
						prevCMD->IsInvalid = true;
					}
					else if ((prevCMD->LBA <= curCMD->LBA) && (prevCMD->EraseEndLBA >= curCMD->LBA)) {
						curCMD->LBA = prevCMD->LBA;
						prevCMD->IsInvalid = true;
					}
				}
			}

		}
		int buf_idx = 0;
		SSDCommand output_array[5];
		for (int nIter = 0; nIter < 5; nIter++) {
			if (cmd_array[nIter].IsInvalid == false) {
				output_array[buf_idx] = cmd_array[nIter];
				buf_idx++;
			}
		}
		for (int nIter = buf_idx; nIter < 5; nIter++) {
			output_array[nIter].IsInvalid = true;
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
