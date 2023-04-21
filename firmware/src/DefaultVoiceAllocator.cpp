#include "DefaultVoiceAllocator.h"

DefaultVoiceAllocator::DefaultVoiceAllocator()
{
    this->reset();
}

void DefaultVoiceAllocator::reset()
{
    for (uint8_t i = 0; i < 16; i++) {
        this->oplVoices[i] = -1;
    }
    for (uint8_t i = 0; i < 24; i++) {
        this->sd1Voices[i] = -1;
    }

    this->keyOffQueue.clear();
    this->keyOnQueue.clear();

    for (uint8_t i = 0; i < 16; i++) {
        this->keyOffQueue.pushBack(i);
    }
}

int8_t DefaultVoiceAllocator::allocateSD1Voice(uint8_t oplVoice)
{
    int8_t sd1Voice = this->sd1Voices[oplVoice];
    if (sd1Voice >= 0) {
        this->keyOffQueue.remove(sd1Voice);
        this->keyOnQueue.remove(sd1Voice);
        this->keyOnQueue.pushBack(sd1Voice);
        return sd1Voice;
    }

    sd1Voice = this->keyOffQueue.popFront();
    if (sd1Voice < 0) {
        sd1Voice = this->keyOnQueue.popFront();
    }

    if (sd1Voice >= 0) {
        int8_t oldOPLVoice = this->oplVoices[sd1Voice];
        if (oldOPLVoice >= 0) {
            this->sd1Voices[oldOPLVoice] = -1;
        }
        this->oplVoices[sd1Voice] = oplVoice;
        this->sd1Voices[oplVoice] = sd1Voice;
        this->keyOnQueue.pushBack(sd1Voice);
    }

    return sd1Voice;
}

int8_t DefaultVoiceAllocator::getSD1VoiceForOPLVoice(uint8_t oplVoice)
{
    if (oplVoice >= 24) {
        return -1;
    }
    return this->sd1Voices[oplVoice];
}

int8_t DefaultVoiceAllocator::getOPLVoiceForSD1Voice(uint8_t sd1Voice)
{
    if (sd1Voice >= 16) {
        return -1;
    }
    return this->oplVoices[sd1Voice];
}

void DefaultVoiceAllocator::setVoiceKeyOff(uint8_t oplVoice)
{
    int8_t sd1Voice = this->sd1Voices[oplVoice];
    if (sd1Voice >= 0) {
        if (this->keyOnQueue.remove(sd1Voice)) {
            this->keyOffQueue.pushBack(sd1Voice);
        }
    }
}

void DefaultVoiceAllocator::releaseVoice(uint8_t oplVoice)
{
    int8_t sd1Voice = this->sd1Voices[oplVoice];
    if (sd1Voice >= 0) {
        this->keyOnQueue.remove(sd1Voice);
        this->keyOffQueue.remove(sd1Voice);
        this->oplVoices[sd1Voice] = -1;
        this->sd1Voices[oplVoice] = -1;
        this->keyOffQueue.pushFront(sd1Voice);
    }
}
