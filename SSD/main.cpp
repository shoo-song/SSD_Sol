#pragma once
#include <iostream>
#include "SSD.cpp"
#include "FileManager.h"
#include "command_parser.h"
//#define UNIT_TEST
using namespace std;
bool parseCommand(int argc, char* argv[], CommandParser& parser)
{
	if (argc == 3) {
		return parser.parseArg(*argv[1], argv[2]);
	}
	else if (argc == 4) {
		return parser.parseArg(*argv[1], argv[2], argv[3]);
	}
	return false;
}
void executeCMD(CommandParser& parser)
{
	SSD MySSD;

	if (parser.GetCmdType() == CMD_WRITE) {
		MySSD.DoWrite(parser.GetLBA(), parser.GetData());
	}
	else if (parser.GetCmdType() == CMD_READ) {
		MySSD.DoRead(parser.GetLBA());
	}
	else if (parser.GetCmdType() == CMD_ERASE) {
		MySSD.DoErase(parser.GetLBA(), parser.GetEraseEndLBA() - parser.GetLBA() + 1);
	}

}
#ifndef UNIT_TEST
int main(int argc, char* argv[]) {
	CommandParser parser;
	bool invalid = false;
	invalid = parseCommand(argc, argv, parser );
	if (invalid == false) return 0;

	executeCMD(parser);

	return 0;
}
#endif
