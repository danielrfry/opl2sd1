#pragma once

#include "SD1ArrayConfiguration.h"
#include <cstdint>

class SD1DeviceArray {
public:
    SD1DeviceArray(const SD1ArrayConfiguration* config);
    void init();
    void reset();
    void write(const uint8_t* data, const uint16_t len, int8_t stackIndex = -1, SD1Channel channel = SD1Channel::ALL);
    void writeReg(uint8_t addr, uint8_t data, int8_t stackIndex = -1, SD1Channel channel = SD1Channel::ALL);

private:
    const SD1ArrayConfiguration* config;
};
