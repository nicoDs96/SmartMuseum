#include <stdio.h>
#include "shell.h"
#include "xtimer.h"
#include "lpsxxx.h"
#include "lpsxxx_params.h"
#include "read_sensors.h"


int read_sensors(uint16_t *pres, int16_t *temp)
{
   
    lpsxxx_t dev;
    //printf("\n\nTest application for %s pressure sensor\n", LPSXXX_SAUL_NAME);
    //printf("Initializing %s sensor\n", LPSXXX_SAUL_NAME);
    if (lpsxxx_init(&dev, &lpsxxx_params[0]) != LPSXXX_OK) {
        puts("Initialization failed");
        *pres=30;
        *temp=2000;
        printf("Pressure value: %ihPa - Temperature(RAW): %d °C \n",
           *pres, *temp);
        return READ_OK;
    }

    lpsxxx_enable(&dev);
    xtimer_sleep(1); /* wait a bit for the measurements to complete */

    lpsxxx_read_temp(&dev,temp); //temp and pres are already pointers
    lpsxxx_read_pres(&dev,pres);
    lpsxxx_disable(&dev); //do not waste energy, disable sensors after measurements

    printf("Pressure value: %ihPa - Temperature(RAW): %d °C \n",
           *pres, *temp);

    /*int temp_abs = *temp / 100;
    *temp -= temp_abs * 100;

    printf("Pressure value: %ihPa - Temperature: %2i.%02i°C\n",
           *pres, temp_abs, *temp);*/
           
    return READ_OK;
}


int read_sensors(uint16_t *hum, int16_t *temp)
{
    uint32_t  	seed = 1;
    random_init(seed);
    *temp = (int16_t) random_uint32_range(11, 46);
    *hum = (uint16_t) random_uint32_range(0, 2101 );
    return READ_OK;

}