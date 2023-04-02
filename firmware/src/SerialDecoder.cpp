#include "SerialDecoder.h"

bool SerialDecoder::putByte(uint8_t b)
{
    bool gotByte = false;

    if ((b & 1) == 0) {
        this->sync = true;
        this->buffer = 0;
        this->bits = 0;
    } else if (this->sync) {
        if (this->bits + 7 >= 8) {
            uint8_t shift = 8 - this->bits;
            this->outputByte = (this->buffer << shift) | (b >> (8 - shift));
            gotByte = true;
        }
        this->bits = (this->bits + 7) % 8;
        this->buffer = (this->buffer << 7) | (b >> 1);
    }

    if (!gotByte) {
        this->outputByte = 0;
    }

    return gotByte;
}

uint8_t SerialDecoder::getOutputByte() const
{
    return this->outputByte;
}
