#pragma once

#include <cstdint>

class IVoiceAllocator {
public:
    virtual void reset() = 0;
    virtual int8_t allocateSD1Voice(uint8_t oplVoice) = 0;
    virtual int8_t getSD1VoiceForOPLVoice(uint8_t oplVoice) = 0;
    virtual int8_t getOPLVoiceForSD1Voice(uint8_t sd1Voice) = 0;
    virtual void setVoiceKeyOff(uint8_t oplVoice) = 0;
    virtual void releaseVoice(uint8_t oplVoice) = 0;
};
