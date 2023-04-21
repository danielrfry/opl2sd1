#include "SD1DeviceArray.h"
#include "SD1OPLAdaptor.h"
#include "SerialDecoder.h"
#include "SerialProtocol.h"
#include "config.h"

#include <pico/stdlib.h>
#include <tusb.h>

SD1DeviceArray sd1(&SD1_ARRAY_CONFIG);
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
    opl.reset();

    SerialDecoder decoder;
    SerialProtocol protocol(&opl);
    uint8_t readPos = 0;

    while (true) {
        tud_task();
        while (tud_cdc_available()) {
            int b = tud_cdc_read_char();
            if (b >= 0) {
                bool gotByte = decoder.putByte(b);
                protocol.setCS(decoder.getCSState());
                if (gotByte) {
                    protocol.putByte(decoder.getOutputByte());
                }
            }
        }
        opl.update();
    }

    return 0;
}
