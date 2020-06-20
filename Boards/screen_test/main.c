#include <stdio.h>

#include "periph/gpio.h"
#include "periph/i2c.h"

#include "xtimer.h"

#include "u8g2.h"
#include "u8x8_riotos.h"

int main(void)
{
    uint32_t screen = 0;
    u8g2_t u8g2;

    
    /* initialize to I2C */
    puts("Initializing to I2C.");

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);

    u8x8_riotos_t user_data =
    {
        .device_index = 0, //0
        .pin_cs = GPIO_UNDEF, //GPIO_UNDEF
        .pin_dc = GPIO_UNDEF, //GPIO_UNDEF
        .pin_reset = GPIO16, //GPIO16
    };

    u8g2_SetUserPtr(&u8g2, &user_data);
    u8g2_SetI2CAddress(&u8g2, 0x3c);


    /* initialize the display */
    puts("Initializing display.");

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    /* start drawing in a loop */
    puts("Drawing on screen.");
    
    while (1) {
        u8g2_FirstPage(&u8g2);

        do {
            u8g2_SetDrawColor(&u8g2, 1);
            u8g2_SetFont(&u8g2, u8g2_font_helvB12_tf);

            switch (screen) {
                case 0:
                    u8g2_DrawStr(&u8g2, 12, 22, "THIS");
                    break;
                case 1:
                    u8g2_DrawStr(&u8g2, 24, 22, "IS");
                    break;
                case 2:
                    u8g2_DrawStr(&u8g2, 1, 22, "NicoDs '");
                    break;
                case 3:
                    u8g2_DrawStr(&u8g2, 1, 22, "BOARD");
                    break;
            }
        } while (u8g2_NextPage(&u8g2));


        /* show screen in next iteration */
        screen = (screen + 1) % 4;

        /* sleep a little */
        xtimer_sleep(1);
    }

    return 0;
}
