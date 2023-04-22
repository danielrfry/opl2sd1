#include "DualSD1VoiceAllocator.h"
#include "OPLUtils.h"

DualSD1VoiceAllocator::DualSD1VoiceAllocator()
{
    this->reset();
}

void DualSD1VoiceAllocator::reset()
{
    for (uint8_t i = 0; i < 24; i++) {
        this->oplVoices[i] = -1;
        this->sd1Voices[i] = -1;
    }
}

int8_t DualSD1VoiceAllocator::allocateSD1Voice(uint8_t oplVoice)
{
    if (oplVoice >= 24) {
        return -1;
    }

    int8_t sd1Voice = this->sd1Voices[oplVoice];
    if (sd1Voice >= 0) {
        return sd1Voice;
    }

    if (oplVoice >= 18) {
        // Voice is 4-op, cancel 2-op voice assignments
        uint8_t opl2OpVoices[2];
        OPL::get2OpChannelsFor4OpChannel(oplVoice - 18, &opl2OpVoices[0]);
        this->sd1Voices[opl2OpVoices[0]] = -1;
        this->sd1Voices[opl2OpVoices[1]] = -1;
    } else {
        // Voice is 2-op, cancel 4-op voice assignment
        uint8_t opl4OpVoice = OPL::get4OpChannelFor2OpChannel(oplVoice);
        if (opl4OpVoice != 255) {
            this->sd1Voices[opl4OpVoice] = -1;
        }
    }

    sd1Voice = oplVoice;
    this->oplVoices[sd1Voice] = oplVoice;
    this->sd1Voices[oplVoice] = sd1Voice;

    return sd1Voice;
}

int8_t DualSD1VoiceAllocator::getSD1VoiceForOPLVoice(uint8_t oplVoice)
{
    if (oplVoice >= 24) {
        return -1;
    }
    return this->sd1Voices[oplVoice];
}

int8_t DualSD1VoiceAllocator::getOPLVoiceForSD1Voice(uint8_t sd1Voice)
{
    if (sd1Voice >= 24) {
        return -1;
    }
    return this->oplVoices[sd1Voice];
}

void DualSD1VoiceAllocator::setVoiceKeyOff(uint8_t oplVoice)
{
}

void DualSD1VoiceAllocator::releaseVoice(uint8_t oplVoice)
{
    this->oplVoices[oplVoice] = -1;
    this->sd1Voices[oplVoice] = -1;
}
