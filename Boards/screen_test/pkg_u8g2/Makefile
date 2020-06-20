APPLICATION=screen_test

USEMODULE += xtimer

USEPKG += u8g2

BOARD=esp32-heltec-lora32-v2

RIOTBASE = ../../RIOT
# set default device parameters in case they are undefined
TEST_OUTPUT ?= 4

TEST_I2C ?= 0

TEST_ADDR ?= 0x3c


TEST_PIN_CS ?= GPIO_UNDEF
TEST_PIN_DC ?= GPIO_UNDEF
TEST_PIN_RESET ?= GPIO16

#ifeq ($(TEST_OUTPUT),3)
#  TEST_DISPLAY ?= u8g2_Setup_ssd1306_128x64_noname_1
#endif

#ifeq ($(TEST_OUTPUT),4)
#  TEST_DISPLAY ?= u8g2_Setup_ssd1306_i2c_128x64_noname_1
#endif

#TEST_DISPLAY=u8g2_Setup_ssd1306_i2c_128x64_noname_f 



FEATURES_REQUIRED += periph_gpio periph_i2c


# export parameters
CFLAGS += -DTEST_OUTPUT=$(TEST_OUTPUT)

CFLAGS += -DTEST_SPI=$(TEST_SPI)
CFLAGS += -DTEST_I2C=$(TEST_I2C)

CFLAGS += -DTEST_ADDR=$(TEST_ADDR)

CFLAGS += -DTEST_PIN_CS=$(TEST_PIN_CS)
CFLAGS += -DTEST_PIN_DC=$(TEST_PIN_DC)
CFLAGS += -DTEST_PIN_RESET=$(TEST_PIN_RESET)

CFLAGS += -DTEST_DISPLAY=$(TEST_DISPLAY)

include $(RIOTBASE)/Makefile.include
