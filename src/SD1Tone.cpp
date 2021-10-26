#include "SD1Tone.h"

#include <cstring>

uint16_t SD1Tone::getParam(uint16_t paramID)
{
    uint8_t opID = paramID / 32;
    uint8_t param = paramID % 32;

    if (opID == 0) {
        switch (param) {
        case SD1_TONE_PARAM_BO:
            return getBO();
        case SD1_TONE_PARAM_LFO:
            return getLFO();
        case SD1_TONE_PARAM_ALG:
            return getALG();
        }
    } else {
        uint8_t op = opID - 1;

        switch (param) {
        case SD1_TONE_PARAM_OP_SR:
            return getSR(op);
        case SD1_TONE_PARAM_OP_XOF:
            return getXOF(op);
        case SD1_TONE_PARAM_OP_KSR:
            return getKSR(op);
        case SD1_TONE_PARAM_OP_RR:
            return getRR(op);
        case SD1_TONE_PARAM_OP_DR:
            return getDR(op);
        case SD1_TONE_PARAM_OP_AR:
            return getAR(op);
        case SD1_TONE_PARAM_OP_SL:
            return getSL(op);
        case SD1_TONE_PARAM_OP_TL:
            return getTL(op);
        case SD1_TONE_PARAM_OP_KSL:
            return getKSL(op);
        case SD1_TONE_PARAM_OP_DAM:
            return getDAM(op);
        case SD1_TONE_PARAM_OP_EAM:
            return getEAM(op);
        case SD1_TONE_PARAM_OP_DVB:
            return getDVB(op);
        case SD1_TONE_PARAM_OP_EVB:
            return getEVB(op);
        case SD1_TONE_PARAM_OP_MULTI:
            return getMULTI(op);
        case SD1_TONE_PARAM_OP_DT:
            return getDT(op);
        case SD1_TONE_PARAM_OP_WS:
            return getWS(op);
        case SD1_TONE_PARAM_OP_FB:
            return getFB(op);
        }
    }

    return 0;
}

void SD1Tone::setParam(uint16_t paramID, uint16_t value)
{
    uint8_t opID = paramID / 32;
    uint8_t param = paramID % 32;

    if (opID == 0) {
        switch (param) {
        case SD1_TONE_PARAM_BO:
            return setBO(clamp<uint8_t>((uint8_t)value, 0, 3));
        case SD1_TONE_PARAM_LFO:
            return setLFO(clamp<uint8_t>((uint8_t)value, 0, 3));
        case SD1_TONE_PARAM_ALG:
            return setALG(clamp<uint8_t>((uint8_t)value, 0, 7));
        }
    } else {
        uint8_t op = opID - 1;

        switch (param) {
        case SD1_TONE_PARAM_OP_SR:
            return setSR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_XOF:
            return setXOF(op, value != 0);
        case SD1_TONE_PARAM_OP_KSR:
            return setKSR(op, value != 0);
        case SD1_TONE_PARAM_OP_RR:
            return setRR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_DR:
            return setDR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_AR:
            return setAR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_SL:
            return setSL(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_TL:
            return setTL(op, clamp<uint8_t>((uint8_t)value, 0, 63));
        case SD1_TONE_PARAM_OP_KSL:
            return setKSL(op, clamp<uint8_t>((uint8_t)value, 0, 3));
        case SD1_TONE_PARAM_OP_DAM:
            return setDAM(op, clamp<uint8_t>((uint8_t)value, 0, 3));
        case SD1_TONE_PARAM_OP_EAM:
            return setEAM(op, value != 0);
        case SD1_TONE_PARAM_OP_DVB:
            return setDVB(op, clamp<uint8_t>((uint8_t)value, 0, 3));
        case SD1_TONE_PARAM_OP_EVB:
            return setEVB(op, value != 0);
        case SD1_TONE_PARAM_OP_MULTI:
            return setMULTI(op, clamp<uint8_t>((uint8_t)value, 0, 15));
        case SD1_TONE_PARAM_OP_DT:
            return setDT(op, clamp<uint8_t>((uint8_t)value, 0, 7));
        case SD1_TONE_PARAM_OP_WS:
            return setWS(op, clamp<uint8_t>((uint8_t)value, 0, 31));
        case SD1_TONE_PARAM_OP_FB:
            return setFB(op, clamp<uint8_t>((uint8_t)value, 0, 7));
        }
    }
}

bool SD1Tone::operator==(const SD1Tone& b) const
{
    return memcmp(&this->toneData, &b.toneData, sizeof(this->toneData)) == 0;
}

bool SD1Tone::operator!=(const SD1Tone& b) const
{
    return memcmp(&this->toneData, &b.toneData, sizeof(this->toneData)) != 0;
}