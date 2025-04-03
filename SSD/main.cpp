#pragma once
#include <iostream>
#include "SSD.cpp"
#include "FileManager.h"
#include "command_parse.h"
#define UNIT_TEST
using namespace std;
bool commandParse(int argc, SSDCommand& InputParam, char* argv[])
{
	if (argc == 3) {
		return InputParam.parseArg(*argv[1], argv[2]);
	}
	else if (argc == 4) {
		return InputParam.parseArg(*argv[1], argv[2], argv[3]);
	}
	return false;
}
void executeCMD(SSDCommand& InputParam)
{
	SSD MySSD;
	if (InputParam.GetCmdType() == CMD_WRITE) {
		MySSD.DoWrite(InputParam.GetLBA(), InputParam.GetData());
	}
	else if (InputParam.GetCmdType() == CMD_READ) {
		MySSD.DoRead(InputParam.GetLBA());
	}
	else if (InputParam.GetCmdType() == CMD_ERASE) {
		MySSD.DoErase(InputParam.GetLBA(), InputParam.GetEraseCount());
	}
}
#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	SSDCommand InputParam;
	bool invalid = false;
	invalid = commandParse(argc, InputParam, argv);
	if (invalid == false) return 0;
	executeCMD(InputParam);

	return 0;
}
#endif
