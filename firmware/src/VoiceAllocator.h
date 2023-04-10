#pragma once

#include <cstdint>

#include "VoiceQueue.h"

class VoiceAllocator {
public:
    VoiceAllocator();
    void reset();
    int8_t getSD1VoiceForOPLVoice(uint8_t oplVoice, bool allocate = false);
    int8_t getOPLVoiceForSD1Voice(uint8_t sd1Voice);
    void setVoiceKeyOff(uint8_t oplVoice);
    void releaseVoice(uint8_t oplVoice);

private:
    int8_t oplVoices[16];
    int8_t sd1Voices[24];
    VoiceQueue<16> keyOffQueue;
    VoiceQueue<16> keyOnQueue;
};
