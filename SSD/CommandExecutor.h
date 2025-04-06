#pragma once

#include "BufferCommand.h"
#include "CommandFileSystem.h"

CmdInfo commandParse(int argc, char *argv[]);

void executeCMD(const CmdInfo &command, BufferCommand &bufferCmd);