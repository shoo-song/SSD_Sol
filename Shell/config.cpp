#include "config.h"

// ���� ���� ���� ���� �� �ʱⰪ ����

namespace config {

const std::string FILE_PATH = "ssd_output.txt";
const size_t BUFFER_SIZE = 4096;
const bool ENABLE_LOGGING = true;
const size_t MAX_LINES = 10000;

const std::string READ_MARKER = "SSD.exe R ";
const std::string WRITE_MARKER = "SSD.exe W ";
const std::string ERASE_MARKER = "SSD.exe E ";
const std::string FLUSH_MARKER = "SSD.exe F ";

}  // namespace config