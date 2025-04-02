#include "gmock/gmock.h"
#include "ssddriver_interface.h"

#ifndef SSD_DRIVER_MOCK_H_
#define SSD_DRIVER_MOCK_H_

class MockSsdDriver : public SsdDriverInterface {
public:
    MOCK_METHOD(void, writeSSD, (int LSD, uint32_t data), (override));
    MOCK_METHOD(uint32_t, readSSD, (int LSD), (override));
};

#endif // !SSD_DRIVER_MOCK_H_