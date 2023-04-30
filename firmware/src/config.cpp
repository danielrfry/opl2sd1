#include "config.h"

#if OPL2SD1_DUAL_STEREO == 1

const SD1DeviceConfiguration SD1_DEVICE_CONFIGS[] = {
    {
        .pinCS = PIN_CS_L,
        .pinCSLED = PIN_LED_CS_L,
        .bank = 0,
        .channel = SD1Channel::LEFT,
    },
    {
        .pinCS = PIN_CS_R,
        .pinCSLED = PIN_LED_CS_R,
        .bank = 0,
        .channel = SD1Channel::RIGHT,
    },
    {
        .pinCS = PIN_CS_L2,
        .pinCSLED = PIN_LED_CS_L2,
        .bank = 1,
        .channel = SD1Channel::LEFT,
    },
    {
        .pinCS = PIN_CS_R2,
        .pinCSLED = PIN_LED_CS_R2,
        .bank = 1,
        .channel = SD1Channel::RIGHT,
    },
};
constexpr uint8_t SD1_NUM_BANKS = 2;

#elif OPL2SD1_STEREO == 1

const SD1DeviceConfiguration SD1_DEVICE_CONFIGS[] = {
    {
        .pinCS = PIN_CS_L,
        .pinCSLED = PIN_LED_CS_L,
        .bank = 0,
        .channel = SD1Channel::LEFT,
    },
    {
        .pinCS = PIN_CS_R,
        .pinCSLED = PIN_LED_CS_R,
        .bank = 0,
        .channel = SD1Channel::RIGHT,
    },
};
constexpr uint8_t SD1_NUM_BANKS = 1;

#else

const SD1DeviceConfiguration SD1_DEVICE_CONFIGS[] = {
    {
        .pinCS = PIN_CS_L,
        .pinCSLED = PIN_LED_CS_L,
        .bank = 0,
        .channel = SD1Channel::BOTH,
    },
};
constexpr uint8_t SD1_NUM_BANKS = 1;

#endif

const SD1ArrayConfiguration SD1_ARRAY_CONFIG = {
    .numDevices = sizeof(SD1_DEVICE_CONFIGS) / sizeof(SD1DeviceConfiguration),
    .numBanks = SD1_NUM_BANKS,
    .devices = &SD1_DEVICE_CONFIGS[0],
};