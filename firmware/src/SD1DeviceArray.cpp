#include "SD1DeviceArray.h"
#include "config.h"

#include <hardware/spi.h>
#include <pico/stdlib.h>

SD1DeviceArray::SD1DeviceArray(const SD1ArrayConfiguration* config) : config(config)
{
}

void SD1DeviceArray::init()
{
    gpio_init(PIN_RST);
    gpio_put(PIN_RST, 0);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    for (uint8_t i = 0; i < this->config->numDevices; i++) {
        gpio_init(this->config->devices[i].pinCS);
        gpio_put(this->config->devices[i].pinCS, 1);
        gpio_set_dir(this->config->devices[i].pinCS, GPIO_OUT);
    }

    spi_init(spi0, 8000000);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_put(PIN_OE, 0);

    this->reset();
}

void SD1DeviceArray::reset()
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

void SD1DeviceArray::write(const uint8_t* data, const uint16_t len, int8_t stackIndex, SD1Channel channel)
{
    uint32_t csMask = 0;
    for (uint8_t i = 0; i < this->config->numDevices; i++) {
        if (stackIndex == -1 || this->config->devices[i].stackIndex == stackIndex) {
            if (channel == SD1Channel::ALL || channel == this->config->devices[i].channel) {
                csMask |= 1 << this->config->devices[i].pinCS;
            }
        }
    }

    gpio_put_masked(csMask, 0);
    spi_write_blocking(spi0, data, len);
    gpio_put_masked(csMask, csMask);
}

void SD1DeviceArray::writeReg(uint8_t addr, uint8_t data, int8_t stackIndex, SD1Channel channel)
{
    uint8_t buffer[2] = { addr, data };
    this->write(&buffer[0], 2, stackIndex, channel);
}
