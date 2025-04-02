#include "gmock/gmock.h"
#include "ssddriver_interface.h"

#ifndef SSD_DRIVER_MOCK_H_
#define SSD_DRIVER_MOCK_H_

#include <string>

class MockSsdDriver : public SsdDriverInterface {
public:
    MOCK_METHOD(bool, executeCmd, (std::string cmdLine), (override));
    MOCK_METHOD(uint32_t, readSSD, (int LBA), (override));
    MOCK_METHOD(void, writeSSD, (int LBA, uint32_t data), (override));
};

#endif // !SSD_DRIVER_MOCK_H_