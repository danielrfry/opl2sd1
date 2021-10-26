#include "OPLTone.h"
#include "OPLUtils.h"

uint16_t OPLTone::getParam(uint16_t paramID)
{
    uint8_t opID = paramID / 16;
    uint8_t param = paramID % 16;

    if (opID == 0) {
        switch (param) {
        case OPL_TONE_PARAM_4OP:
            return is4Op();
        case OPL_TONE_PARAM_FB:
            return getFB();
        case OPL_TONE_PARAM_CNT:
            return getCNT();
        }
    } else if (opID >= 1 && opID <= 4) {
        uint8_t op = opID - 1;

        switch (param) {
        case OPL_TONE_PARAM_OP_AM:
            return getAM(op);
        case OPL_TONE_PARAM_OP_VIB:
            return getVIB(op);
        case OPL_TONE_PARAM_OP_EGT:
            return getEGT(op);
        case OPL_TONE_PARAM_OP_KSR:
            return getKSR(op);
        case OPL_TONE_PARAM_OP_MULT:
            return getMULT(op);
        case OPL_TONE_PARAM_OP_KSL:
            return getKSL(op);
        case OPL_TONE_PARAM_OP_TL:
            return getTL(op);
        case OPL_TONE_PARAM_OP_AR:
            return getAR(op);
        case OPL_TONE_PARAM_OP_DR:
            return getDR(op);
        case OPL_TONE_PARAM_OP_SL:
            return getSL(op);
        case OPL_TONE_PARAM_OP_RR:
            return getRR(op);
        case OPL_TONE_PARAM_OP_WS:
            return getWS(op);
        }
    }

    return 0;
}

void OPLTone::setParam(uint16_t paramID, uint16_t value)
{
    uint8_t opID = paramID / 16;
    uint8_t param = paramID % 16;

    if (opID == 0) {
        switch (param) {
        case OPL_TONE_PARAM_4OP:
            set4Op(value != 0);
            break;
        case OPL_TONE_PARAM_FB:
            setFB(clamp<uint8_t>((uint8_t)value, 0, 7));
            break;
        case OPL_TONE_PARAM_CNT:
            setCNT(clamp<uint8_t>((uint8_t)value, 0, 3));
            break;
        }
    } else if (opID >= 1 && opID <= 4) {
        uint8_t op = opID - 1;

        switch (param) {
        case OPL_TONE_PARAM_OP_AM:
            setAM(op, value != 0);
            break;
        case OPL_TONE_PARAM_OP_VIB:
            setVIB(op, value != 0);
            break;
        case OPL_TONE_PARAM_OP_EGT:
            setEGT(op, value != 0);
            break;
        case OPL_TONE_PARAM_OP_KSR:
            setKSR(op, value != 0);
            break;
        case OPL_TONE_PARAM_OP_MULT:
            setMULT(op, clamp<uint8_t>((uint8_t)value, 0, 15));
            break;
        case OPL_TONE_PARAM_OP_KSL:
            setKSL(op, clamp<uint8_t>((uint8_t)value, 0, 3));
            break;
        case OPL_TONE_PARAM_OP_TL:
            setTL(op, clamp<uint8_t>((uint8_t)value, 0, 63));
            break;
        case OPL_TONE_PARAM_OP_AR:
            setAR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
            break;
        case OPL_TONE_PARAM_OP_DR:
            setDR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
            break;
        case OPL_TONE_PARAM_OP_SL:
            setSL(op, clamp<uint8_t>((uint8_t)value, 0, 15));
            break;
        case OPL_TONE_PARAM_OP_RR:
            setRR(op, clamp<uint8_t>((uint8_t)value, 0, 15));
            break;
        case OPL_TONE_PARAM_OP_WS:
            setWS(op, clamp<uint8_t>((uint8_t)value, 0, 7));
            break;
        }
    }
}

uint8_t OPLTone::getCarriers(uint8_t carriers[])
{
    if (this->is4Op()) {
        switch (this->getCNT()) {
        case 0:
            carriers[0] = 3;
            return 1;
        case 1:
            carriers[0] = 1;
            carriers[1] = 3;
            return 2;
        case 2:
            carriers[0] = 0;
            carriers[1] = 3;
            return 2;
        case 3:
            carriers[0] = 0;
            carriers[1] = 2;
            carriers[2] = 3;
            return 3;
        }
    } else {
        if (this->getCNT() & 1) {
            carriers[0] = 0;
            carriers[1] = 1;
            return 2;
        } else {
            carriers[0] = 1;
            return 1;
        }
    }

    return 0;
}
