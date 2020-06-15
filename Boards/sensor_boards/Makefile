# name of your application
APPLICATION = museum_boards

# If no BOARD is found in the environment, use this default:
BOARD ?= iotlab-m3
# if no other driver is specified
DRIVER ?= lps331ap
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

BOARD_INSUFFICIENT_MEMORY := arduino-duemilanove arduino-nano arduino-uno

# include the shell:
USEMODULE += shell
USEMODULE += shell_commands
# additional modules for debugging:
USEMODULE += ps
#TIMING
USEMODULE += xtimer
#Sensors
USEMODULE += $(DRIVER)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
