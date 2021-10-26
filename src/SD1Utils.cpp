#include "SD1Utils.h"

void SD1::convertOPLTone(OPLTone& oplTone, SD1Tone& sd1Tone)
{
    uint8_t numOps = oplTone.is4Op() ? 4 : 2;

    for (uint8_t op = 0; op < numOps; op++) {
        sd1Tone.setBO(1);
        sd1Tone.setEAM(op, oplTone.getAM(op));
        sd1Tone.setEVB(op, oplTone.getVIB(op));
        if (oplTone.getEGT(op)) {
            sd1Tone.setSR(op, 0);
        } else {
            sd1Tone.setSR(op, oplTone.getRR(op));
        }
        sd1Tone.setKSR(op, oplTone.getKSR(op));
        sd1Tone.setMULTI(op, oplTone.getMULT(op));
        sd1Tone.setKSL(op, oplTone.getKSL(op));
        sd1Tone.setTL(op, oplTone.getTL(op));
        sd1Tone.setAR(op, oplTone.getAR(op));
        sd1Tone.setDR(op, oplTone.getDR(op));
        sd1Tone.setSL(op, oplTone.getSL(op));
        sd1Tone.setRR(op, oplTone.getRR(op));
        sd1Tone.setWS(op, oplTone.getWS(op));
        sd1Tone.setFB(op, op == 0 ? oplTone.getFB() : 0);
    }

    if (oplTone.is4Op()) {
        sd1Tone.setALG(oplTone.getCNT() + 4);
    } else {
        sd1Tone.setALG(oplTone.getCNT());
    }
}
