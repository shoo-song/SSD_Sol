#include "gmock/gmock.h"
#include "ssddriver.h"
#include "ssddriver_interface.h"

#ifndef SSD_DRIVER_MOCK_H_
#define SSD_DRIVER_MOCK_H_

#include <string>

class MockSsdDriver : public SsdDriver {
   public:
    MOCK_METHOD(uint32_t, readSSD, (int LBA), (override));
    MOCK_METHOD(void, writeSSD, (int LBA, uint32_t data), (override));
    MOCK_METHOD(bool, executeCmd, (std::string cmdLine), (override));
};

#endif  // !SSD_DRIVER_MOCK_H_