#pragma once

#include <cstdint>

enum class SD1Channel : uint8_t {
    LEFT = 1,
    RIGHT = 2,
    BOTH = LEFT | RIGHT
};

inline SD1Channel operator|(SD1Channel a, SD1Channel b)
{
    return static_cast<SD1Channel>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline SD1Channel operator&(SD1Channel a, SD1Channel b)
{
    return static_cast<SD1Channel>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

class SD1Device {
public:
    void init();
    void reset();
    void write(const uint8_t* data, const uint16_t len, SD1Channel channel = SD1Channel::BOTH);
    void writeReg(uint8_t addr, uint8_t data, SD1Channel channel = SD1Channel::BOTH);
};
