#pragma once

#include "IVoiceAllocator.h"

class DualSD1VoiceAllocator : public IVoiceAllocator {
public:
    DualSD1VoiceAllocator();
    virtual void reset() override;
    virtual int8_t allocateSD1Voice(uint8_t oplVoice) override;
    virtual int8_t getSD1VoiceForOPLVoice(uint8_t oplVoice) override;
    virtual int8_t getOPLVoiceForSD1Voice(uint8_t sd1Voice) override;
    virtual void setVoiceKeyOff(uint8_t oplVoice) override;
    virtual void releaseVoice(uint8_t oplVoice) override;

private:
    int8_t oplVoices[24];
    int8_t sd1Voices[24];
};
