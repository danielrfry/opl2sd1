#include "config.h"

#if OPL2SD1_STEREO == 1

const SD1DeviceConfiguration SD1_DEVICE_CONFIGS[] = {
    {
        .pinCS = PIN_CS_L,
        .stackIndex = 0,
        .channel = SD1Channel::LEFT,
    },
    {
        .pinCS = PIN_CS_R,
        .stackIndex = 0,
        .channel = SD1Channel::RIGHT,
    },
};

#else

const SD1DeviceConfiguration SD1_DEVICE_CONFIGS[] = {
    {
        .pinCS = PIN_CS_L,
        .stackIndex = 0,
        .channel = SD1Channel::BOTH,
    },
};

#endif

const SD1ArrayConfiguration SD1_ARRAY_CONFIG = {
    .numDevices = sizeof(SD1_DEVICE_CONFIGS) / sizeof(SD1DeviceConfiguration),
    .devices = &SD1_DEVICE_CONFIGS[0],
};