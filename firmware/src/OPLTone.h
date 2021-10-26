#pragma once

#include <cstdint>

#include "Utils.h"

#define OPL_TONE_FLAG_4OP 1

#define OPL_TONE_PARAM_4OP 0
#define OPL_TONE_PARAM_FB 1
#define OPL_TONE_PARAM_CNT 2

#define OPL_TONE_PARAM_OP_AM 0
#define OPL_TONE_PARAM_OP_VIB 1
#define OPL_TONE_PARAM_OP_EGT 2
#define OPL_TONE_PARAM_OP_KSR 3
#define OPL_TONE_PARAM_OP_MULT 4
#define OPL_TONE_PARAM_OP_KSL 5
#define OPL_TONE_PARAM_OP_TL 6
#define OPL_TONE_PARAM_OP_AR 7
#define OPL_TONE_PARAM_OP_DR 8
#define OPL_TONE_PARAM_OP_SL 9
#define OPL_TONE_PARAM_OP_RR 10
#define OPL_TONE_PARAM_OP_WS 11

typedef struct __attribute__((__packed__)) OPLOperatorData {
    uint8_t avekm; // 0x20-0x35: AM/VIB/EGT/KSR/MULT
    uint8_t ksl_tl; // 0x40-0x55: KSL/TL
    uint8_t ar_dr; // 0x60-0x75: AR/DR
    uint8_t sl_rr; // 0x80-0x95: SL/RR
    uint8_t ws; // 0xe0-0xf5: WS
} OPLOperatorData;

typedef struct __attribute__((__packed__)) OPLToneData {
    uint8_t flags;
    OPLOperatorData op[4];
    uint8_t fbcnt[2];
} OPLToneData;

class OPLTone {
public:
    OPLToneData toneData;

    uint16_t getParam(uint16_t paramID);
    void setParam(uint16_t paramID, uint16_t value);

    inline bool getAM(uint8_t op)
    {
        return getBit<7>(toneData.op[op].avekm);
    }
    inline void setAM(uint8_t op, bool value)
    {
        setBit<7>(toneData.op[op].avekm, value);
    }
    inline bool getVIB(uint8_t op)
    {
        return getBit<6>(toneData.op[op].avekm);
    }
    inline void setVIB(uint8_t op, bool value)
    {
        setBit<6>(toneData.op[op].avekm, value);
    }
    inline bool getEGT(uint8_t op)
    {
        return getBit<5>(toneData.op[op].avekm);
    }
    inline void setEGT(uint8_t op, bool value)
    {
        setBit<5>(toneData.op[op].avekm, value);
    }
    inline bool getKSR(uint8_t op)
    {
        return getBit<4>(toneData.op[op].avekm);
    }
    inline void setKSR(uint8_t op, bool value)
    {
        setBit<4>(toneData.op[op].avekm, value);
    }
    inline uint8_t getMULT(uint8_t op)
    {
        return getBits<0, 4>(toneData.op[op].avekm);
    }
    inline void setMULT(uint8_t op, uint8_t value)
    {
        setBits<0, 4>(toneData.op[op].avekm, value);
    }
    inline uint8_t getKSL(uint8_t op)
    {
        return getBits<6, 2>(toneData.op[op].ksl_tl);
    }
    inline void setKSL(uint8_t op, uint8_t value)
    {
        setBits<6, 2>(toneData.op[op].ksl_tl, value);
    }
    inline uint8_t getTL(uint8_t op)
    {
        return getBits<0, 6>(toneData.op[op].ksl_tl);
    }
    inline void setTL(uint8_t op, uint8_t value)
    {
        setBits<0, 6>(toneData.op[op].ksl_tl, value);
    }
    inline uint8_t getAR(uint8_t op)
    {
        return getBits<4, 4>(toneData.op[op].ar_dr);
    }
    inline void setAR(uint8_t op, uint8_t value)
    {
        setBits<4, 4>(toneData.op[op].ar_dr, value);
    }
    inline uint8_t getDR(uint8_t op)
    {
        return getBits<0, 4>(toneData.op[op].ar_dr);
    }
    inline void setDR(uint8_t op, uint8_t value)
    {
        setBits<0, 4>(toneData.op[op].ar_dr, value);
    }
    inline uint8_t getSL(uint8_t op)
    {
        return getBits<4, 4>(toneData.op[op].sl_rr);
    }
    inline void setSL(uint8_t op, uint8_t value)
    {
        setBits<4, 4>(toneData.op[op].sl_rr, value);
    }
    inline uint8_t getRR(uint8_t op)
    {
        return getBits<0, 4>(toneData.op[op].sl_rr);
    }
    inline void setRR(uint8_t op, uint8_t value)
    {
        setBits<0, 4>(toneData.op[op].sl_rr, value);
    }
    inline uint8_t getWS(uint8_t op)
    {
        return getBits<0, 3>(toneData.op[op].ws);
    }
    inline void setWS(uint8_t op, uint8_t value)
    {
        setBits<0, 3>(toneData.op[op].ws, value);
    }

    inline uint8_t getFB()
    {
        return getBits<1, 3>(toneData.fbcnt[0]);
    }
    inline void setFB(uint8_t value)
    {
        setBits<1, 3>(toneData.fbcnt[0], value);
    }

    inline uint8_t getCNT()
    {
        if (is4Op()) {
            return (getBit<0>(toneData.fbcnt[1]) ? 2 : 0)
                | (getBit<0>(toneData.fbcnt[0]) ? 1 : 0);
        } else {
            return getBit<0>(toneData.fbcnt[0]) ? 1 : 0;
        }
    }
    inline void setCNT(uint8_t value)
    {
        if (is4Op()) {
            setBit<0>(toneData.fbcnt[1], (value & 2) != 0);
            setBit<0>(toneData.fbcnt[0], (value & 1) != 0);
        } else {
            setBit<0>(toneData.fbcnt[0], (value & 1) != 0);
        }
    }

    inline bool is4Op()
    {
        return (toneData.flags & OPL_TONE_FLAG_4OP) != 0;
    }
    inline void set4Op(bool value)
    {
        if (value) {
            toneData.flags |= OPL_TONE_FLAG_4OP;
        } else {
            toneData.flags &= ~OPL_TONE_FLAG_4OP;
        }
    }

    uint8_t getCarriers(uint8_t carriers[]);
};
