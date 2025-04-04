#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <string>

namespace config {
    extern const std::string FILE_PATH;
    extern const size_t BUFFER_SIZE;
    extern const bool ENABLE_LOGGING;
    extern const size_t MAX_LINES;

    extern const std::string READ_MARKER;
    extern const std::string WRITE_MARKER;
    extern const std::string ERASE_MARKER;
    extern const std::string FLUSH_MARKER;
}

#endif // !CONFIG_H_