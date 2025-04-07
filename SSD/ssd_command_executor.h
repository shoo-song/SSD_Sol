#pragma once

#include "ssd_buffer_command.h"
#include "ssd_command_file_system.h"

CmdInfo commandParse(int argc, char *argv[]);

void executeCMD(const CmdInfo &command, BufferCommand &bufferCmd);