#pragma once

#include <cstdint>

enum class SD1Channel : uint8_t {
    LEFT = 1, // Addresses chips that output to left channel
    RIGHT = 2, // Addresses chips that output to right channel
    BOTH = 3, // Addresses chips that output to both channels
    ALL = 4 // Addresses all chips
};

class SD1DeviceConfiguration {
public:
    uint8_t pinCS;
    uint8_t stackIndex;
    SD1Channel channel;
};

class SD1ArrayConfiguration {
public:
    uint8_t numDevices;
    const SD1DeviceConfiguration* devices;
};
