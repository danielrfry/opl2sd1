#pragma once

#include <cstdint>

#include "IVoiceAllocator.h"
#include "VoiceQueue.h"

class DefaultVoiceAllocator : public IVoiceAllocator {
public:
    DefaultVoiceAllocator();
    virtual void reset() override;
    virtual int8_t allocateSD1Voice(uint8_t oplVoice) override;
    virtual int8_t getSD1VoiceForOPLVoice(uint8_t oplVoice) override;
    virtual int8_t getOPLVoiceForSD1Voice(uint8_t sd1Voice) override;
    virtual void setVoiceKeyOff(uint8_t oplVoice) override;
    virtual void releaseVoice(uint8_t oplVoice) override;

private:
    int8_t oplVoices[16];
    int8_t sd1Voices[24];
    VoiceQueue<16> keyOffQueue;
    VoiceQueue<16> keyOnQueue;
};
