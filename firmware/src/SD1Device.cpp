#include "SD1Device.h"
#include "config.h"

#include <hardware/spi.h>
#include <pico/stdlib.h>

void SD1Device::init()
{
    gpio_init(PIN_RST);
    gpio_put(PIN_RST, 0);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_init(PIN_CS_L);
    gpio_put(PIN_CS_L, 1);
    gpio_set_dir(PIN_CS_L, GPIO_OUT);

#if OPL2SD1_STEREO == 1
    gpio_init(PIN_CS_R);
    gpio_put(PIN_CS_R, 1);
    gpio_set_dir(PIN_CS_R, GPIO_OUT);
#endif

    spi_init(spi0, 8000000);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_put(PIN_OE, 0);

    this->reset();
}

void SD1Device::reset()
{
    gpio_put(PIN_RST, 0);
    sleep_ms(1);
    gpio_put(PIN_RST, 1);
    this->writeReg(0x1D, 0); // DRV_SEL (power rail selection: single 5V supply)
    this->writeReg(0x02, 0x0E); // AP (analog block power control)
    sleep_ms(1);
    this->writeReg(0x00, 0x01); // CLKEN
    this->writeReg(0x01, 0x00); // ALRST
    this->writeReg(0x1A, 0xA3); // SFTRST (soft reset)
    sleep_ms(1);
    this->writeReg(0x1A, 0x00);
    sleep_ms(30);
    this->writeReg(0x02, 0x04); // AP1,AP3
    sleep_ms(1);
    this->writeReg(0x02, 0x00); // AP2
    // add
    this->writeReg(0x19, 0xB0); // MASTER VOL
    this->writeReg(0x1B, 0x00); // interpolation
    this->writeReg(0x14, 0x01); // interpolation
    this->writeReg(0x03, 0x01); // Analog Gain

    this->writeReg(0x08, 0xF6); // Reset sequencer
    sleep_ms(21);
    this->writeReg(0x08, 0x00); // Clear reset
    this->writeReg(0x09, 0xF8); // Sequencer volume
    this->writeReg(0x0A, 0x00); // Sequence data size

    this->writeReg(0x17, 0x40); // MS_S
    this->writeReg(0x18, 0x00);
}

void SD1Device::write(const uint8_t* data, const uint16_t len, SD1Channel channel)
{
    bool enableL = (channel & SD1Channel::LEFT) == SD1Channel::LEFT;
    bool enableR = (channel & SD1Channel::RIGHT) == SD1Channel::RIGHT;

#if OPL2SD1_STEREO == 1
    constexpr uint32_t BIT_CS_L = 1 << PIN_CS_L;
    constexpr uint32_t BIT_CS_R = 1 << PIN_CS_R;
    gpio_put_masked(BIT_CS_L | BIT_CS_R, (enableL ? 0 : BIT_CS_L) | (enableR ? 0 : BIT_CS_R));
#else
    gpio_put(PIN_CS_L, (enableL | enableR) ? 0 : 1);
#endif

    spi_write_blocking(spi0, data, len);

    gpio_put(PIN_CS_L, 1);
#if OPL2SD1_STEREO == 1
    gpio_put_masked(BIT_CS_L | BIT_CS_R, BIT_CS_L | BIT_CS_R);
#endif
}

void SD1Device::writeReg(uint8_t addr, uint8_t data, SD1Channel channel)
{
    uint8_t buffer[2] = { addr, data };
    this->write(&buffer[0], 2, channel);
}
