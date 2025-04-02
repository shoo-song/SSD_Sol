#ifndef SSD_DRIVER
#define SSD_DRIVER
#include "ssddriver_interface.h"

class SsdDriver : public SsdDriverInterface {
public:

    uint32_t readSSD(int LBA);

    void writeSSD(int LBA, uint32_t data);
        
};

#endif // SSD_DRIVER
