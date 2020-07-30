# name of your application
APPLICATION = emcute_mqttsn

# If no BOARD is found in the environment, use this default:
BOARD ?= esp32-heltec-lora32-v2
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/nicods/Scrivania/SmartMuseum/Boards/RIOT
# WIFI config
USEMODULE += esp_wifi
#CFLAGS += -DESP_WIFI_SSID=\"D-Link-5544FC\"
#CFLAGS += -DESP_WIFI_PASS=\"xGSVxnnyYh\"
CFLAGS += -DESP_WIFI_SSID=\"dsnetwork-TIM-31161311\"
CFLAGS += -DESP_WIFI_PASS=\"DiSaNtONM\"

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
#USEMODULE += gnrc_netdev_default
#USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += ipv4_addr
USEMODULE += gnrc_sock_udp
#USEMODULE += gnrc_ipv6_default
# Include MQTT-SN
USEMODULE += emcute
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
# For testing we also include the ping6 command and some stats
USEMODULE += icmp_echo

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Comment this out to join RPL DODAGs even if DIOs do not contain
# DODAG Configuration Options (see the doc for more info)
# CFLAGS += -DCONFIG_GNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
