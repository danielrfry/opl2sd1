#include "SD1Device.h"
#include "SD1OPLAdaptor.h"
#include "config.h"

#include <pico/stdlib.h>
#include <tusb.h>

SD1Device sd1;
SD1OPLAdaptor opl(&sd1);

void init_levelshift()
{
    gpio_init(PIN_OE);
    gpio_put(PIN_OE, 1);
    gpio_set_dir(PIN_OE, GPIO_OUT);
}

int main()
{
    tusb_init();
    init_levelshift();

    sd1.init();

    uint8_t packet[3];
    uint8_t readPos = 0;

    while (true) {
        tud_task();
        while (tud_cdc_available() > 0) {
            tud_cdc_read(&packet[readPos], 1);
            readPos++;

            if (readPos == 3) {
                readPos = 0;
                if (packet[0] == 0xff) {
                    if (packet[1] == 0x00 && packet[2] == 0x01) {
                        opl.reset();
                    }
                } else if (packet[0] == 0x00 || packet[0] == 0x01) {
                    uint16_t addr = (packet[0] << 8) | packet[1];
                    uint8_t value = packet[2];
                    opl.write(addr, value);
                }
            }
        }
        opl.update();
    }

    return 0;
}
