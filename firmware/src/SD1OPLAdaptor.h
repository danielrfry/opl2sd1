#pragma once

#include "DefaultVoiceAllocator.h"
#include "DualSD1VoiceAllocator.h"
#include "IOPLDevice.h"
#include "OPLRegisterSet.h"
#include "SD1DeviceArray.h"
#include "SD1Tone.h"

class SD1OPLAdaptor : public IOPLDevice {
public:
    SD1OPLAdaptor(SD1DeviceArray* device);
    virtual void reset();
    virtual void write(uint16_t addr, uint8_t data);
    void update();

private:
    SD1DeviceArray* device;
    OPLRegisterSet oplReg;
    SD1Tone tones[24];
    bool changes[24];
    DefaultVoiceAllocator defaultVoiceAllocator;
    DualSD1VoiceAllocator dualSD1VoiceAllocator;
    IVoiceAllocator* voiceAllocator;
    uint8_t activeBank;

    void initState();
    void updateTones(uint8_t bank);
    void updatePitch();
    void sd1WriteTones(uint8_t bank, uint8_t numTones);
    void oplGetBlockFNum(uint8_t oplVoice, uint8_t& block, uint16_t& fnum);
    void sd1SelectVoice(uint8_t voice);
    void sd1SetPitch(uint8_t block, uint16_t fnum);
    void handleOPLFNUMLChange(uint16_t addr, uint8_t oldData);
    void handleOPLKONBlockFNUMHChange(uint16_t addr, uint8_t oldData);
    void handleOPLConnSelChange(uint8_t oldData);
    void handleOPLOutputChannelsChange(uint8_t oplVoice);
    bool getOPL3Mode();
    void handleOPL3ModeChange();
    uint8_t getOPLOutputChannels(uint8_t oplVoice);
    void sd1SetOutputChannels(uint8_t channels);
    void sd1SetKeyOn(bool on, uint8_t tone, bool egRst);
    void resetVoice(uint8_t oplVoice);
};
