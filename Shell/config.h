#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <string>

namespace config {
    extern const std::string FILE_PATH;
    extern const size_t BUFFER_SIZE;
    extern const bool ENABLE_LOGGING;
    extern const size_t MAX_LINES;
}

#endif // !CONFIG_H_