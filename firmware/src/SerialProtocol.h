#pragma once

#include <cstdint>

#include "IOPLDevice.h"

enum class SerialProtocolState {
    EXPECT_DEVICE_ID,
    EXPECT_FIRST_REG,
    EXPECT_PORT,
    EXPECT_VALUE,
    IGNORE
};

class SerialProtocol {
public:
    SerialProtocol(IOPLDevice* opl3);
    void setCS(bool cs);
    void putByte(uint8_t b);

private:
    IOPLDevice* opl3;
    bool cs = false;
    uint16_t reg = 0;
    uint8_t port = 0;
    SerialProtocolState state;
};
