#pragma once

enum ShellCommand {
    SSD_READ_COMMAND,
    SSD_WRITE_COMMAND,
    SSD_HELP_COMMAND,
    SSD_EXIT_COMMAND,
    SSD_FULLWRITE_COMMAND,
    SSD_FULLREAD_COMMAND,
    SSD_ERASE_COMMAND,
    SSD_ERASERANGE_COMMAND,
    SSD_FLUSH_COMMAND,
    SSD_SCRIPT_RUN_COMMAND
};
enum ShellScriptApiCommand {
    WRITE_SCRIPT_COMMAND,
    COMPARE_SCRIPT_COMMAND,
    ERASE_SCRIPT_COMMAND,
    FLUSH_SCRIPT_COMMAND,
    SCRIPT_UNKOWN
};
