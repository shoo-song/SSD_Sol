#pragma once
#include <iostream>
#include "SSD.cpp"
#include "File_Interface.h"
#include "command_parse.h"

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
	SSDReadDriver Reader;
	SSDWriteDriver Writer;
	if (InputParam.IsWrite() == true) {
		Writer.DoWrite(InputParam.GetLBA(), InputParam.GetData());
	}
	else {
		Reader.DoRead(InputParam.GetLBA());
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
