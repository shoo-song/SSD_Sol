#ifndef SSD_DRIVER
#define SSD_DRIVER
#include "ssddriver_interface.h"
#include <string>

class SsdDriver : public SsdDriverInterface {
public:

    uint32_t readSSD(int LBA);

    void writeSSD(int LBA, uint32_t data);

    bool executeCmd(std::string cmdLine);

private:
    std::string TrimData(const std::string& hexStr);
    uint32_t ConvertStrToUint32(const std::string& hexStr);
};



#endif // SSD_DRIVER
