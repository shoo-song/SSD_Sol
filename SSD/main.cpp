#pragma once
#include <iostream>
#include "SSD.cpp"
//#include "FileManager.h"
#include "command_parser.h"
#include "BufferCommand.h"

using namespace std;
CmdInfo commandParse(int argc, char* argv[])
{
	CommandParser parser;
	CmdInfo ErrorCmd;
	if (argc == 3) {
		return parser.parseArg(argc, *argv[1], argv[2]);
	}
	else if (argc == 4) {
		return parser.parseArg(argc , *argv[1], argv[2], argv[3]);
	}
	ErrorCmd.IsValid = false;
	return ErrorCmd;
}
void executeCMD(CmdInfo Command)
{
	BufferCommand CMDBuf;
	CMDBuf.PushCommand(Command);
}
#ifndef UNIT_TEST
int main(int argc, char* argv[]) {

	CmdInfo Command = commandParse(argc, argv);
	if (!Command.IsValid) {
		return 0;
	}
	executeCMD(Command);
	return 0;
}
#endif
