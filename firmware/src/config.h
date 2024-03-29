#pragma once

#if !defined(OPL2SD1_STEREO)
#define OPL2SD1_STEREO 0
#endif

#define PIN_SCK 2
#define PIN_MOSI 3

// If stereo is not enabled, only PIN_CS_L is used
#define PIN_CS_L 6
#define PIN_CS_R 7

#define PIN_RST 4
#define PIN_OE 5
