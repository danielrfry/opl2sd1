#include "SerialProtocol.h"

SerialProtocol::SerialProtocol(IOPLDevice* opl3)
    : opl3(opl3), state(SerialProtocolState::IGNORE)
{
}

void SerialProtocol::setCS(bool cs)
{
    if (this->cs != cs) {
        this->cs = cs;
        if (this->cs) {
            this->state = SerialProtocolState::EXPECT_DEVICE_ID;
        }
    }
}

void SerialProtocol::putByte(uint8_t b)
{
    if (!this->cs) {
        return;
    }

    switch (this->state) {
    case SerialProtocolState::EXPECT_DEVICE_ID:
        if (b == 0x42) {
            this->state = SerialProtocolState::EXPECT_FIRST_REG;
        } else {
            this->state = SerialProtocolState::IGNORE;
        }
        break;
    case SerialProtocolState::EXPECT_FIRST_REG:
        if (b == 0x12) {
            this->state = SerialProtocolState::EXPECT_PORT;
        } else {
            this->state = SerialProtocolState::IGNORE;
        }
        break;
    case SerialProtocolState::EXPECT_PORT:
        if (b == 0xfe) {
            this->opl3->reset();
            this->state = SerialProtocolState::IGNORE;
        } else if (b == 0xff) {
            this->state = SerialProtocolState::IGNORE;
        } else {
            this->port = b;
            this->state = SerialProtocolState::EXPECT_VALUE;
        }
        break;
    case SerialProtocolState::EXPECT_VALUE:
        if (this->port == 0xe1) {
            this->reg = b;
        } else if (this->port == 0xe5) {
            this->reg = 0x100 | b;
        } else if (this->port == 0xfb) {
            this->opl3->write(this->reg, b);
        }
        this->state = SerialProtocolState::EXPECT_PORT;
    default:
        break;
    };
}
