#pragma once

#include "SD1ArrayConfiguration.h"

#if !defined(OPL2SD1_QUAD)
#define OPL2SD1_QUAD 0
#endif

#if !defined(OPL2SD1_STEREO)
#define OPL2SD1_STEREO 0
#endif

#define PIN_SCK 2
#define PIN_MOSI 3

// If stereo is not enabled, only PIN_CS_L is used
#define PIN_CS_L 6
#define PIN_CS_R 7

// These pins are only used when quad SD-1 is enabled
#define PIN_CS_L2 8
#define PIN_CS_R2 9

#define PIN_RST 4
#define PIN_OE 5

extern const SD1ArrayConfiguration SD1_ARRAY_CONFIG;
