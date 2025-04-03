#ifndef SSD_DRIVER_INTERFACE_H_
#define SSD_DRIVER_INTERFACE_H_

#include <cstdint>
#include <string>

class SsdDriverInterface {
public:
    virtual uint32_t readSSD(int LBA) = 0;
    virtual void writeSSD(int LBA, uint32_t data) = 0;
    //virtual bool executeCmd(std::string cmdLine) = 0;
};

#endif // !
