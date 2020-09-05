#include <stdio.h>
#include "shell.h"
#include "xtimer.h"
#include "read_sensors.h"
#include "random.h"


int read_sensors(uint16_t *hum, int16_t *temp)
{
    uint32_t  	seed = 1;
    random_init(seed);
    *temp = (int16_t) random_uint32_range(1000, 4446);
    *hum = (uint16_t) random_uint32_range(0, 100 );
    return READ_OK;

}