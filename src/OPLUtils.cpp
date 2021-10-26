#include "OPLUtils.h"

#include <algorithm>
#include <math.h>

void OPL::get2OpChannelsFor4OpChannel(uint8_t ch4Op, uint8_t* ch2Op)
{
    ch2Op[0] = ((ch4Op / 3) * 9) + (ch4Op % 3);
    ch2Op[1] = ch2Op[0] + 3;
}

void OPL::get2OpRegOffsets(uint8_t ch2Op, uint16_t* regOffsets)
{
    uint16_t regBase = ch2Op >= 9 ? 0x100 : 0;
    ch2Op = ch2Op % 9;
    regBase += ((ch2Op / 3) * 8) + (ch2Op % 3);
    regOffsets[0] = regBase;
    regOffsets[1] = regBase + 3;
}

uint16_t OPL::get2OpChannelRegOffset(uint8_t ch2Op)
{
    return ch2Op > 8 ? (ch2Op % 9) | 0x100 : (ch2Op % 9);
}

uint8_t OPL::get4OpChannelFor2OpChannel(uint8_t ch2op)
{
    switch (ch2op) {
    case 0:
    case 3:
        return 0;
    case 1:
    case 4:
        return 1;
    case 2:
    case 5:
        return 2;
    case 9:
    case 12:
        return 3;
    case 10:
    case 13:
        return 4;
    case 11:
    case 14:
        return 5;
    default:
        return 255;
    }
}

void OPL::getOperatorRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets)
{
    if (voice > 17) {
        // 4-op channel
        numOffsets = 4;
        uint8_t ch4Op = voice - 18;
        uint8_t ch2Op[2];
        OPL::get2OpChannelsFor4OpChannel(ch4Op, &ch2Op[0]);
        OPL::get2OpRegOffsets(ch2Op[0], &regOffsets[0]);
        OPL::get2OpRegOffsets(ch2Op[1], &regOffsets[2]);
    } else {
        // 2-op channel
        numOffsets = 2;
        OPL::get2OpRegOffsets(voice, &regOffsets[0]);
    }
}

void OPL::getChannelRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets)
{
    if (voice > 17) {
        // 4-op channel
        numOffsets = 2;
        uint8_t ch2Op[2];
        OPL::get2OpChannelsFor4OpChannel(voice - 18, &ch2Op[0]);
        regOffsets[0] = OPL::get2OpChannelRegOffset(ch2Op[0]);
        regOffsets[1] = OPL::get2OpChannelRegOffset(ch2Op[1]);
    } else {
        // 2-op channel
        numOffsets = 1;
        regOffsets[0] = OPL::get2OpChannelRegOffset(voice);
    }
}

uint8_t OPL::get2OpChannelForOperatorRegOffset(uint16_t offset)
{
    uint8_t offsetLSB = offset & 0xff;
    uint8_t voice = (offsetLSB / 8) * 3;
    voice += (offsetLSB % 8) % 3;
    if (offset & 0x100) {
        voice += 9;
    }
    return voice;
}

uint8_t OPL::getVoiceForOperatorRegOffset(uint16_t offset, uint8_t connection)
{
    uint8_t ch2Op = OPL::get2OpChannelForOperatorRegOffset(offset);
    uint8_t ch4Op = OPL::get4OpChannelFor2OpChannel(ch2Op);
    if (ch4Op == 255) {
        return ch2Op;
    } else {
        uint8_t chBit = 1 << ch4Op;
        if (connection & chBit) {
            return 18 + ch4Op;
        } else {
            return ch2Op;
        }
    }
}

uint8_t OPL::getVoiceForChannelRegOffset(uint16_t offset, uint8_t connection)
{
    uint8_t ch2Op = (offset & 0xff);
    if (offset & 0x100) {
        ch2Op += 9;
    }
    uint8_t ch4Op = OPL::get4OpChannelFor2OpChannel(ch2Op);
    if (ch4Op == 255) {
        return ch2Op;
    } else {
        uint8_t chBit = 1 << ch4Op;
        if (connection & chBit) {
            return 18 + ch4Op;
        } else {
            return ch2Op;
        }
    }
}

uint8_t OPL::getVoiceForRegister(uint16_t reg, uint8_t connection)
{
    uint8_t regLSB = reg & 0xff;
    if (regLSB >= 0x20 && regLSB <= 0x35) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x20, connection);
    } else if (regLSB >= 0x40 && regLSB <= 0x55) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x40, connection);
    } else if (regLSB >= 0x60 && regLSB <= 0x75) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x60, connection);
    } else if (regLSB >= 0x80 && regLSB <= 0x95) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0x80, connection);
    } else if (regLSB >= 0xa0 && regLSB <= 0xa8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xa0, connection);
    } else if (regLSB >= 0xb0 && regLSB <= 0xb8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xb0, connection);
    } else if (regLSB >= 0xc0 && regLSB <= 0xc8) {
        return OPL::getVoiceForChannelRegOffset(reg - 0xc0, connection);
    } else if (regLSB >= 0xe0 && regLSB <= 0xf5) {
        return OPL::getVoiceForOperatorRegOffset(reg - 0xe0, connection);
    } else {
        return 255;
    }
}

void OPL::readTone(OPLRegisterSet& registers, uint8_t voice, OPLTone& tone)
{
    uint16_t chRegOffsets[2];
    uint8_t numChannels;
    OPL::getChannelRegOffsetsForVoice(voice, &chRegOffsets[0], numChannels);
    uint16_t opRegOffsets[4];
    uint8_t numOperators;
    OPL::getOperatorRegOffsetsForVoice(voice, &opRegOffsets[0], numOperators);

    bool opl3Enabled = registers.get(0x105) & 1;

    tone = OPLTone();
    if (numOperators == 4) {
        tone.toneData.flags = OPL_TONE_FLAG_4OP;
    } else {
        tone.toneData.flags = 0;
    }

    for (uint8_t ch = 0; ch < numChannels; ch++) {
        tone.toneData.fbcnt[ch] = registers.get(0xc0 + chRegOffsets[ch]) & 0x0f;
    }
    for (uint8_t op = 0; op < numOperators; op++) {
        OPLOperatorData& opData = tone.toneData.op[op];
        opData.avekm = registers.get(0x20 + opRegOffsets[op]);
        opData.ksl_tl = registers.get(0x40 + opRegOffsets[op]);
        opData.ar_dr = registers.get(0x60 + opRegOffsets[op]);
        opData.sl_rr = registers.get(0x80 + opRegOffsets[op]);
        opData.ws = registers.get(0xe0 + opRegOffsets[op]) & (opl3Enabled ? 0x07 : 0x03);
    }
}
