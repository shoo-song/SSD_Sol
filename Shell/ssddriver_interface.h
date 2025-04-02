#ifndef SSD_DRIVER_INTERFACE_H_
#define SSD_DRIVER_INTERFACE_H_

#include <cstdint>

class SsdDriverInterface {
    virtual uint32_t readSSD(int LBA) = 0;
    virtual void writeSSD(int LBA, uint32_t data) = 0;
};

#endif // !
