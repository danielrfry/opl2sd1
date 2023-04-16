#include "SD1OPLAdaptor.h"
#include "OPLUtils.h"
#include "SD1Utils.h"

#include <algorithm>
#include <pico/stdlib.h>

#define SD1_VOICE_ON_VOLUME 0x3c

SD1OPLAdaptor::SD1OPLAdaptor(SD1Device* device) : device(device), oplReg(), tones(), changes(), voiceAllocator()
{
    this->initState();
}

void SD1OPLAdaptor::initState()
{
    for (uint8_t v = 0; v < 24; v++) {
        this->changes[v] = true;
    }
    this->voiceAllocator.reset();
    oplReg.reset();
}

void SD1OPLAdaptor::reset()
{
    device->reset();
    this->initState();
    for (uint8_t v = 0; v < 16; v++) {
        this->sd1SelectVoice(v);
        this->device->writeReg(0x0c, SD1_VOICE_ON_VOLUME);
        this->device->writeReg(0x12, 0x08);
        this->device->writeReg(0x13, 0x24);
    }
}

void SD1OPLAdaptor::write(uint16_t addr, uint8_t data)
{
    uint8_t oldData = this->oplReg.get(addr);
    this->oplReg.set(addr, data);

    if (addr == 0x104) {
        this->handleOPLConnSelChange(oldData);
    } else if (addr == 0x105) {
        if (data != oldData) {
            this->handleOPL3ModeChange();
        }
    } else {
        uint8_t oplVoice = OPL::getVoiceForRegister(addr, this->oplReg.get(0x104));

        uint8_t addrLSB = addr & 0xff;
        if (addrLSB >= 0xa0 && addrLSB <= 0xa8) {
            this->handleOPLFNUMLChange(addr, oldData);
        } else if (addrLSB >= 0xb0 && addrLSB <= 0xb8) {
            this->handleOPLKONBlockFNUMHChange(addr, oldData);
        } else if (addrLSB >= 0xc0 && addrLSB <= 0xc8) {
            this->handleOPLOutputChannelsChange(oplVoice);
        } else if (oplVoice < 24) {
            this->changes[oplVoice] = data != oldData;
        }
    }
}

void SD1OPLAdaptor::update()
{
    int8_t lastChangedTone = -1;
    for (uint8_t v = 0; v < 16; v++) {
        int8_t oplVoice = this->voiceAllocator.getOPLVoiceForSD1Voice(v);
        if (oplVoice >= 0 && this->changes[oplVoice]) {
            OPLTone oplTone;
            OPL::readTone(this->oplReg, oplVoice, oplTone);
            SD1::convertOPLTone(oplTone, this->tones[v]);
            lastChangedTone = std::max(lastChangedTone, (int8_t)v);
        }
    }

    this->sd1WriteTones(lastChangedTone);

    // Update pitch
    for (uint8_t v = 0; v < 16; v++) {
        int8_t oplVoice = this->voiceAllocator.getOPLVoiceForSD1Voice(v);
        if (oplVoice >= 0 && this->changes[oplVoice]) {
            uint8_t block;
            uint16_t fnum;
            this->oplGetBlockFNum(oplVoice, block, fnum);
            this->sd1SelectVoice(v);
            this->sd1SetPitch(block, fnum);
            this->changes[oplVoice] = false;
        }
    }
}

void SD1OPLAdaptor::sd1WriteTones(int8_t maxTone)
{
    if (maxTone < 0)
        return;

    uint8_t buffer[486];
    buffer[0] = 0x07;
    buffer[1] = 0x81 + maxTone;
    uint16_t offset = 2;
    for (uint8_t toneIndex = 0; toneIndex <= maxTone; toneIndex++) {
        SD1ToneData* toneData = (SD1ToneData*)((void*)&buffer[offset]);
        *toneData = this->tones[toneIndex].toneData;
        offset += sizeof(SD1ToneData);
    }
    buffer[offset++] = 0x80;
    buffer[offset++] = 0x03;
    buffer[offset++] = 0x81;
    buffer[offset++] = 0x80;

    this->device->writeReg(0x08, 0x16);
    sleep_us(7);
    this->device->writeReg(0x08, 0x00);
    this->device->write(&buffer[0], offset);
}

void SD1OPLAdaptor::oplGetBlockFNum(uint8_t oplVoice, uint8_t& block, uint16_t& fnum)
{
    uint16_t regOffsets[2];
    uint8_t numOffsets;
    OPL::getChannelRegOffsetsForVoice(oplVoice, &regOffsets[0], numOffsets);

    uint8_t fnumL = this->oplReg.get(0xa0 + regOffsets[0]);
    uint16_t blockFNumH = this->oplReg.get(0xb0 + regOffsets[0]);
    fnum = fnumL | ((blockFNumH & 0x03) << 8);
    block = (blockFNumH & 0x1c) >> 2;
}

void SD1OPLAdaptor::sd1SelectVoice(uint8_t voice)
{
    this->device->writeReg(0x0b, voice);
}

void SD1OPLAdaptor::sd1SetPitch(uint8_t block, uint16_t fnum)
{
    this->device->writeReg(0x0d, ((fnum & 0x380) >> 4) | (block & 0x07));
    this->device->writeReg(0x0e, fnum & 0x7f);
}

void SD1OPLAdaptor::handleOPLFNUMLChange(uint16_t addr, uint8_t oldData)
{
    uint16_t offset = addr - 0xa0;
    uint8_t oplVoice = OPL::getVoiceForChannelRegOffset(offset, this->oplReg.get(0x104));
    uint8_t data = this->oplReg.get(addr);
    if (data != oldData) {
        this->changes[oplVoice] = true;
    }
}

void SD1OPLAdaptor::handleOPLKONBlockFNUMHChange(uint16_t addr, uint8_t oldData)
{
    uint16_t offset = addr - 0xb0;
    uint8_t oplVoice = OPL::getVoiceForChannelRegOffset(offset, this->oplReg.get(0x104));
    uint8_t data = this->oplReg.get(addr);

    if ((data & 0x1f) != (oldData & 0x1f)) {
        // Pitch changed
        this->changes[oplVoice] = true;
    }

    bool newKeyOn = (data & 0x20) != 0;
    bool oldKeyOn = (oldData & 0x20) != 0;

    if (newKeyOn != oldKeyOn) {
        int8_t sd1Voice = this->voiceAllocator.getSD1VoiceForOPLVoice(oplVoice);
        bool initSD1Voice = false;
        if (newKeyOn) {
            int8_t newSD1Voice = this->voiceAllocator.allocateSD1Voice(oplVoice);
            // Initialise voice parameters if the assignment changed
            initSD1Voice = newSD1Voice != sd1Voice;
            sd1Voice = newSD1Voice;
            this->changes[oplVoice] = true;
        } else {
            this->voiceAllocator.setVoiceKeyOff(oplVoice);
        }

        this->update();

        if (sd1Voice >= 0) {
            this->sd1SelectVoice(sd1Voice);
            if (initSD1Voice) {
                this->sd1SetKeyOn(false, sd1Voice, true);
                this->sd1SetOutputChannels(this->getOPLOutputChannels(oplVoice));
            }
            this->sd1SetKeyOn((data & 0x20) != 0, sd1Voice, false);
        }
    }
}

void SD1OPLAdaptor::handleOPLConnSelChange(uint8_t oldData)
{
    uint8_t data = this->oplReg.get(0x104);
    if (data == oldData)
        return;

    for (uint8_t b = 0; b < 6; b++) {
        uint8_t mask = 1 << b;
        if ((data & mask) && !(oldData & mask)) {
            // Channel was 2-op, now 4-op. Deallocate 2-op channels
            uint8_t ch2Op[2];
            OPL::get2OpChannelsFor4OpChannel(b, &ch2Op[0]);
            this->resetVoice(ch2Op[0]);
            this->resetVoice(ch2Op[1]);
        }
        if (!(data & mask) && (oldData & mask)) {
            // Channel was 4-op, now 2-op. Deallocate 4-op channel
            this->resetVoice(b + 18);
        }
    }
}

void SD1OPLAdaptor::handleOPLOutputChannelsChange(uint8_t oplVoice)
{
    int8_t sd1Voice = this->voiceAllocator.getSD1VoiceForOPLVoice(oplVoice);
    if (sd1Voice >= 0) {
        this->sd1SelectVoice(sd1Voice);
        this->sd1SetOutputChannels(this->getOPLOutputChannels(oplVoice));
    }
}

bool SD1OPLAdaptor::getOPL3Mode()
{
    return (this->oplReg.get(0x105) & 0x01) != 0;
}

void SD1OPLAdaptor::handleOPL3ModeChange()
{
    for (uint8_t v = 0; v < 24; v++) {
        this->handleOPLOutputChannelsChange(v);
    }
}

uint8_t SD1OPLAdaptor::getOPLOutputChannels(uint8_t oplVoice)
{
    if (this->getOPL3Mode()) {
        uint16_t regOffsets[2];
        uint8_t numOffsets;
        OPL::getChannelRegOffsetsForVoice(oplVoice, &regOffsets[0], numOffsets);
        return (this->oplReg.get(0xc0 + regOffsets[0]) & 0x30) >> 4;
    } else {
        return 3;
    }
}

void SD1OPLAdaptor::sd1SetOutputChannels(uint8_t channels)
{
    switch (channels) {
    case 0:
        this->device->writeReg(0x0c, 0x00, SD1Channel::BOTH);
        break;
    case 1:
        this->device->writeReg(0x0c, SD1_VOICE_ON_VOLUME, SD1Channel::LEFT);
        this->device->writeReg(0x0c, 0x00, SD1Channel::RIGHT);
        break;
    case 2:
        this->device->writeReg(0x0c, 0x00, SD1Channel::LEFT);
        this->device->writeReg(0x0c, SD1_VOICE_ON_VOLUME, SD1Channel::RIGHT);
        break;
    case 3:
        this->device->writeReg(0x0c, SD1_VOICE_ON_VOLUME, SD1Channel::BOTH);
        break;
    }
}

void SD1OPLAdaptor::sd1SetKeyOn(bool on, uint8_t tone, bool egRst)
{
    uint8_t data = tone;
    if (on) {
        data |= 0x40;
    }
    if (egRst) {
        data |= 0x10;
    }
    this->device->writeReg(0x0f, data);
}

void SD1OPLAdaptor::resetVoice(uint8_t oplVoice)
{
    int8_t sd1Voice = this->voiceAllocator.getSD1VoiceForOPLVoice(oplVoice);
    if (sd1Voice >= 0) {
        this->sd1SelectVoice(sd1Voice);
        this->sd1SetKeyOn(false, sd1Voice, true);
        this->voiceAllocator.releaseVoice(oplVoice);
    }
}