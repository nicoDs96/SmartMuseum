# name of your application
APPLICATION = museum_boards

# If no BOARD is found in the environment, use this default:
BOARD ?= esp32-heltec-lora32-v2  #b-l072z-lrwan1
# if no other driver is specified
DRIVER ?= lps331ap
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/nicods/Scrivania/SmartMuseum/Boards/RIOT   #$(CURDIR)/../../

#OLED SCREEN MODULE
#USEPKG += u8g2

#WIFI Module
#USEMODULE += esp_wifi
#CFLAGS += -DESP_WIFI_SSID=\"D-Link-5544FC\"
#CFLAGS += -DESP_WIFI_PASS=\"xGSVxnnyYh\"
#CFLAGS += -DESP_WIFI_SSID=\"dsnetwork-TIM-31161311\"
#CFLAGS += -DESP_WIFI_PASS=\"DiSaNtONM\"


# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default
# Include MQTT-SN
USEMODULE += emcute
# For testing we also include the ping6 command and some stats
USEMODULE += gnrc_icmpv6_echo

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# Optimize network stack to for use with a single network interface
CFLAGS += -DGNRC_NETIF_SINGLE


# include the shell:
USEMODULE += shell
USEMODULE += shell_commands

# additional modules for debugging:
USEMODULE += ps

#TIMING
USEMODULE += xtimer

#Sensors
USEMODULE += $(DRIVER)

#LoRa
LORA_DRIVER ?= sx1276
LORA_REGION ?= EU868
USEPKG += semtech-loramac
USEMODULE += $(LORA_DRIVER)
USEMODULE += fmt
USEMODULE += semtech_loramac_rx
CFLAGS += -DREGION_$(LORA_REGION)
CFLAGS += -DLORAMAC_ACTIVE_REGION=LORAMAC_REGION_$(LORA_REGION)


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
