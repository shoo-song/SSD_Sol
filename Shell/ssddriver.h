#ifndef SSD_DRIVER
#define SSD_DRIVER
#include "ssddriver_interface.h"

class SsdDriver : public SsdDriverInterface {
public:

    uint32_t readSSD(int LSD);

    
    void writeSSD(int LSD, uint32_t data);
        
};

#endif // SSD_DRIVER
