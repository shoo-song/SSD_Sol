#pragma once
#include <iostream>
#include "SSD.cpp"
#include "File_Interface.h"
#include "command_parse.h"

using namespace std;

void commandParse(int argc, SSDCommand& InputParam, char* argv[])
{
	if (argc == 3) {
		InputParam.parseArg(*argv[1], argv[2]);
	}
	else if (argc == 4) {
		InputParam.parseArg(*argv[1], argv[2], argv[3]);
	}
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
int flag = 0;
int main(int argc, char* argv[]) {
	SSDCommand InputParam;

	commandParse(argc, InputParam, argv);
	executeCMD(InputParam);

	return 0;
}
