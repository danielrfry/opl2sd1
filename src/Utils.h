#pragma once

#include <cstdint>

template <uint8_t bit>
inline bool getBit(uint8_t data)
{
    return (data & (1 << bit)) != 0;
}

template <uint8_t start, uint8_t count>
inline uint8_t getBits(uint8_t data)
{
    return (data >> start) & ((1 << count) - 1);
}

template <uint8_t bit>
inline void setBit(uint8_t& data, bool value)
{
    data = (data & ~(1 << bit)) | ((value & 1) << bit);
}

template <uint8_t start, uint8_t count>
inline void setBits(uint8_t& data, uint8_t value)
{
    uint8_t mask = ((1 << count) - 1) << start;
    data &= ~mask;
    data |= (value << start) & mask;
}

template <typename T>
inline T clamp(T value, T min, T max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}
