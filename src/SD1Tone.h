#pragma once

#include "Utils.h"

#include <cstdint>

#define SD1_TONE_PARAM_BO 0
#define SD1_TONE_PARAM_LFO 1
#define SD1_TONE_PARAM_ALG 2

#define SD1_TONE_PARAM_OP_SR 0
#define SD1_TONE_PARAM_OP_XOF 1
#define SD1_TONE_PARAM_OP_KSR 2
#define SD1_TONE_PARAM_OP_RR 3
#define SD1_TONE_PARAM_OP_DR 4
#define SD1_TONE_PARAM_OP_AR 5
#define SD1_TONE_PARAM_OP_SL 6
#define SD1_TONE_PARAM_OP_TL 7
#define SD1_TONE_PARAM_OP_KSL 8
#define SD1_TONE_PARAM_OP_DAM 9
#define SD1_TONE_PARAM_OP_EAM 10
#define SD1_TONE_PARAM_OP_DVB 11
#define SD1_TONE_PARAM_OP_EVB 12
#define SD1_TONE_PARAM_OP_MULTI 13
#define SD1_TONE_PARAM_OP_DT 14
#define SD1_TONE_PARAM_OP_WS 15
#define SD1_TONE_PARAM_OP_FB 16

typedef struct __attribute__((__packed__)) SD1OperatorData {
    uint8_t sr_xof_ksr;
    uint8_t rr_dr;
    uint8_t ar_sl;
    uint8_t tl_ksl;
    uint8_t dam_eam_dvb_evb;
    uint8_t multi_dt;
    uint8_t ws_fb;
} SD1OperatorData;

typedef struct __attribute__((__packed__)) SD1ToneData {
    uint8_t bo;
    uint8_t lfo_alg;
    SD1OperatorData op[4];
} SD1ToneData;

class SD1Tone {
public:
    SD1ToneData toneData;

    uint16_t getParam(uint16_t paramID);
    void setParam(uint16_t paramID, uint16_t value);
    bool operator==(const SD1Tone& b) const;
    bool operator!=(const SD1Tone& b) const;

    inline uint8_t getBO()
    {
        return getBits<0, 2>(this->toneData.bo);
    }
    inline void setBO(uint8_t value)
    {
        setBits<0, 2>(this->toneData.bo, value);
    }
    inline uint8_t getLFO()
    {
        return getBits<6, 2>(this->toneData.lfo_alg);
    }
    inline void setLFO(uint8_t value)
    {
        setBits<6, 2>(this->toneData.lfo_alg, value);
    }
    inline uint8_t getALG()
    {
        return getBits<0, 3>(this->toneData.lfo_alg);
    }
    inline void setALG(uint8_t value)
    {
        setBits<0, 3>(this->toneData.lfo_alg, value);
    }
    inline uint8_t getSR(uint8_t opIndex)
    {
        return getBits<4, 4>(this->toneData.op[opIndex].sr_xof_ksr);
    }
    inline void setSR(uint8_t opIndex, uint8_t value)
    {
        setBits<4, 4>(this->toneData.op[opIndex].sr_xof_ksr, value);
    }
    inline bool getXOF(uint8_t opIndex)
    {
        return getBit<3>(this->toneData.op[opIndex].sr_xof_ksr);
    }
    inline void setXOF(uint8_t opIndex, bool value)
    {
        setBit<3>(this->toneData.op[opIndex].sr_xof_ksr, value);
    }
    inline bool getKSR(uint8_t opIndex)
    {
        return getBit<0>(this->toneData.op[opIndex].sr_xof_ksr);
    }
    inline void setKSR(uint8_t opIndex, bool value)
    {
        setBit<0>(this->toneData.op[opIndex].sr_xof_ksr, value);
    }
    inline uint8_t getRR(uint8_t opIndex)
    {
        return getBits<4, 4>(this->toneData.op[opIndex].rr_dr);
    }
    inline void setRR(uint8_t opIndex, uint8_t value)
    {
        setBits<4, 4>(this->toneData.op[opIndex].rr_dr, value);
    }
    inline uint8_t getDR(uint8_t opIndex)
    {
        return getBits<0, 4>(this->toneData.op[opIndex].rr_dr);
    }
    inline void setDR(uint8_t opIndex, uint8_t value)
    {
        setBits<0, 4>(this->toneData.op[opIndex].rr_dr, value);
    }
    inline uint8_t getAR(uint8_t opIndex)
    {
        return getBits<4, 4>(this->toneData.op[opIndex].ar_sl);
    }
    inline void setAR(uint8_t opIndex, uint8_t value)
    {
        setBits<4, 4>(this->toneData.op[opIndex].ar_sl, value);
    }
    inline uint8_t getSL(uint8_t opIndex)
    {
        return getBits<0, 4>(this->toneData.op[opIndex].ar_sl);
    }
    inline void setSL(uint8_t opIndex, uint8_t value)
    {
        setBits<0, 4>(this->toneData.op[opIndex].ar_sl, value);
    }
    inline uint8_t getTL(uint8_t opIndex)
    {
        return getBits<2, 6>(this->toneData.op[opIndex].tl_ksl);
    }
    inline void setTL(uint8_t opIndex, uint8_t value)
    {
        setBits<2, 6>(this->toneData.op[opIndex].tl_ksl, value);
    }
    inline uint8_t getKSL(uint8_t opIndex)
    {
        return getBits<0, 2>(this->toneData.op[opIndex].tl_ksl);
    }
    inline void setKSL(uint8_t opIndex, uint8_t value)
    {
        setBits<0, 2>(this->toneData.op[opIndex].tl_ksl, value);
    }
    inline uint8_t getDAM(uint8_t opIndex)
    {
        return getBits<5, 2>(this->toneData.op[opIndex].dam_eam_dvb_evb);
    }
    inline void setDAM(uint8_t opIndex, uint8_t value)
    {
        setBits<5, 2>(this->toneData.op[opIndex].dam_eam_dvb_evb, value);
    }
    inline bool getEAM(uint8_t opIndex)
    {
        return getBit<4>(this->toneData.op[opIndex].dam_eam_dvb_evb);
    }
    inline void setEAM(uint8_t opIndex, bool value)
    {
        setBit<4>(this->toneData.op[opIndex].dam_eam_dvb_evb, value);
    }
    inline uint8_t getDVB(uint8_t opIndex)
    {
        return getBits<1, 2>(this->toneData.op[opIndex].dam_eam_dvb_evb);
    }
    inline void setDVB(uint8_t opIndex, uint8_t value)
    {
        setBits<1, 2>(this->toneData.op[opIndex].dam_eam_dvb_evb, value);
    }
    inline bool getEVB(uint8_t opIndex)
    {
        return getBit<0>(this->toneData.op[opIndex].dam_eam_dvb_evb);
    }
    inline void setEVB(uint8_t opIndex, bool value)
    {
        setBit<0>(this->toneData.op[opIndex].dam_eam_dvb_evb, value);
    }
    inline uint8_t getMULTI(uint8_t opIndex)
    {
        return getBits<4, 4>(this->toneData.op[opIndex].multi_dt);
    }
    inline void setMULTI(uint8_t opIndex, uint8_t value)
    {
        setBits<4, 4>(this->toneData.op[opIndex].multi_dt, value);
    }
    inline uint8_t getDT(uint8_t opIndex)
    {
        return getBits<0, 3>(this->toneData.op[opIndex].multi_dt);
    }
    inline void setDT(uint8_t opIndex, uint8_t value)
    {
        setBits<0, 3>(this->toneData.op[opIndex].multi_dt, value);
    }
    inline uint8_t getWS(uint8_t opIndex)
    {
        return getBits<3, 5>(this->toneData.op[opIndex].ws_fb);
    }
    inline void setWS(uint8_t opIndex, uint8_t value)
    {
        setBits<3, 5>(this->toneData.op[opIndex].ws_fb, value);
    }
    inline uint8_t getFB(uint8_t opIndex)
    {
        return getBits<0, 3>(this->toneData.op[opIndex].ws_fb);
    }
    inline void setFB(uint8_t opIndex, uint8_t value)
    {
        setBits<0, 3>(this->toneData.op[opIndex].ws_fb, value);
    }
};
