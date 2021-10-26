#pragma once

#include "config.h"

#include <cstdint>

class SD1Device {
public:
    void init();
    void reset();
    void write(const uint8_t* data, const uint16_t len);
    void writeReg(uint8_t addr, uint8_t data);
};
