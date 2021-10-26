#pragma once

#include <cstdint>

class OPLRegisterSet {
public:
    OPLRegisterSet();
    void set(uint16_t addr, uint8_t value);
    uint8_t get(uint16_t addr);
    void reset();

private:
    uint8_t registers[288];
};
