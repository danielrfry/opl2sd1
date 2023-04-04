#pragma once

#include <cstdint>

class IOPLDevice {
public:
    virtual void reset() = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
};
