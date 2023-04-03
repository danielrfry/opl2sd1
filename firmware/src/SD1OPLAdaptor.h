#pragma once

#include "IOPLDevice.h"
#include "OPLRegisterSet.h"
#include "SD1Device.h"
#include "SD1Tone.h"

class SD1OPLAdaptor : public IOPLDevice {
public:
    SD1OPLAdaptor(SD1Device* device);
    virtual void reset();
    virtual void write(uint16_t addr, uint8_t data);
    void update();

private:
    SD1Device* device;
    OPLRegisterSet oplReg;
    SD1Tone tones[16];
    bool changes[24];
    int8_t sd1Voices[24];
    int8_t oplVoices[16];
    uint8_t nextSD1Voice;

    void initState();
    void sd1WriteTones(int8_t maxTone);
    void oplGetBlockFNum(uint8_t oplVoice, uint8_t& block, uint16_t& fnum);
    void sd1SelectVoice(uint8_t voice);
    void sd1SetPitch(uint8_t block, uint16_t fnum);
    void handleOPLFNUMLChange(uint16_t addr, uint8_t oldData);
    void handleOPLKONBlockFNUMHChange(uint16_t addr, uint8_t oldData);
    void sd1SetKeyOn(bool on, uint8_t tone, bool egRst);
    bool allocateSD1Voice(uint8_t oplVoice);
};
