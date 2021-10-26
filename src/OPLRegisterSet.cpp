#include "OPLRegisterSet.h"

#include <cstring>

static int getArrayIndexForRegister(uint16_t addr)
{
    uint8_t reg = addr & 0xff;
    uint8_t bank = (addr & 0x100) >> 8;
    int index = -1;

    if (reg >= 0x01 && reg <= 0x05) {
        index = reg - 1;
    } else if (reg == 0x08) {
        index = reg - 3;
    } else if (reg >= 0x20 && reg <= 0x35) {
        index = reg - 26;
    } else if (reg >= 0x40 && reg <= 0x55) {
        index = reg - 36;
    } else if (reg >= 0x60 && reg <= 0x75) {
        index = reg - 46;
    } else if (reg >= 0x80 && reg <= 0x95) {
        index = reg - 56;
    } else if (reg >= 0xa0 && reg <= 0xa8) {
        index = reg - 66;
    } else if (reg >= 0xb0 && reg <= 0xb8) {
        index = reg - 73;
    } else if (reg == 0xbd) {
        index = reg - 77;
    } else if (reg >= 0xc0 && reg <= 0xc8) {
        index = reg - 79;
    } else if (reg >= 0xe0 && reg <= 0xf5) {
        index = reg - 102;
    }

    if (index >= 0) {
        index += bank * 144;
    }

    return index;
}

OPLRegisterSet::OPLRegisterSet()
{
    this->reset();
}

void OPLRegisterSet::reset()
{
    memset(&this->registers[0], 0, 288);
}

void OPLRegisterSet::set(uint16_t addr, uint8_t value)
{
    int index = getArrayIndexForRegister(addr);
    if (index >= 0) {
        this->registers[index] = value;
    }
}

uint8_t OPLRegisterSet::get(uint16_t addr)
{
    int index = getArrayIndexForRegister(addr);
    if (index < 0) {
        return 0;
    } else {
        return this->registers[index];
    }
}
