#pragma once

#include <cstdint>

class SerialDecoder {
public:
    bool putByte(uint8_t b);
    uint8_t getOutputByte() const;

private:
    uint8_t buffer = 0;
    uint8_t outputByte = 0;
    bool sync = false;
    uint8_t bits = 0;
};
