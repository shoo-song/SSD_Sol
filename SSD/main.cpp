#pragma once
#include <iostream>
#include "SSD.cpp"
#include "BufferCommand.h"
#include "command_parser.h"
#include "BufferCommand.h"

using namespace std;


CmdInfo commandParse(int argc, char* argv[])
{
	CommandParser parser;
	CmdInfo ErrorCmd;

	char CMD = *argv[1];  // argv[1]이 존재하는 경우에만 참조
	std::string LBAstring = (argc >= 3) ? std::string(argv[2]) : "";  // 없는 경우 빈 문자열
	char* data = (argc >= 4) ? argv[3] : nullptr;  // 없는 경우 nullptr

	//return parser.parseArg(argc, *argv[1], argv[2], argv[3]);
	return parser.parseArg(argc, CMD, LBAstring, data);
	ErrorCmd.IsValid = false;
	return ErrorCmd;
}
void executeCMD(CmdInfo Command)
{
	CommandFileSystem fs;
	BufferCommand CMDBuf(fs);
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
