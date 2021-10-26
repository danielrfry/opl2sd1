#pragma once

#include "OPLRegisterSet.h"
#include "OPLTone.h"

#include <cstdint>

namespace OPL {
void get2OpChannelsFor4OpChannel(uint8_t ch4op, uint8_t* ch2op);
void get2OpRegOffsets(uint8_t ch2op, uint16_t* regOffsets);
uint16_t get2OpChannelRegOffset(uint8_t ch2op);
uint8_t get4OpChannelFor2OpChannel(uint8_t ch2op);
void getOperatorRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets);
void getChannelRegOffsetsForVoice(uint8_t voice, uint16_t* regOffsets, uint8_t& numOffsets);
uint8_t get2OpChannelForOperatorRegOffset(uint16_t offset);
uint8_t getVoiceForOperatorRegOffset(uint16_t offset, uint8_t connection);
uint8_t getVoiceForChannelRegOffset(uint16_t offset, uint8_t connection);
uint8_t getVoiceForRegister(uint16_t reg, uint8_t connection);
void readTone(OPLRegisterSet& registers, uint8_t voice, OPLTone& tone);
}
