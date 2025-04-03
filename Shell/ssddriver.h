#ifndef SSD_DRIVER
#define SSD_DRIVER
#include "ssddriver_interface.h"
#include <string>

class SsdDriver : public SsdDriverInterface {
public:
    uint32_t readSSD(int LBA);
    void writeSSD(int LBA, uint32_t data);
    void eraseSSD(int LBA, int size);
    virtual bool executeCmd(std::string cmdLine);
};
#endif // SSD_DRIVER
