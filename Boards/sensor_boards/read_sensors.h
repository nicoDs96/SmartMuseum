#ifndef HEADER_FILE_SENSORS
#define HEADER_FILE_SENSORS

int read_sensors(uint16_t *pres, int16_t *temp);
#define READ_OK  0
#define READ_FAIL 1

#endif