#ifndef LORA_LIB
#define LORA_LIB

#define CONNECTION_FAILED  1
#define CONNECTION_OK  0

#define DEVICE_RATE  6 // 1 to 12

#define PUB_OK  0
#define PUB_FAIL  1

#define RECV_MSG_QUEUE                   (4U)

/* 
define the required keys for OTAA, e.g over-the-air activation (the
null arrays need to be updated with valid LoRa values) 
*/
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = {0x00, 0x8B, 0x51, 0x44,
                                                   0x3D, 0x92, 0xFC, 0x6F};
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = {0x70, 0xB3, 0xD5, 0x7E,
                                                   0xD0, 0x02, 0xD5, 0x8A};
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {0xD1, 0x4D, 0x14, 0xAC,
                                                   0x89, 0xA4, 0xF8, 0x03,
                                                   0x02, 0x31, 0x07, 0x50,
                                                   0x89, 0x78, 0x8A, 0x84};

#endif