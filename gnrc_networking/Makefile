# name of your application
APPLICATION = gnrc_networking
# CFLAGS += -DCONFIG_IEEE802154_DEFAULT_SUBGHZ_CHANNEL=0
# CFLAGS += -DCONFIG_IEEE802154_DEFAULT_SUBGHZ_PAGE=2
# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Activate ICMPv6 error messages
USEMODULE += gnrc_icmpv6_error
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp

FEATURES_REQUIRED = periph_rtc

DISABLE_MODULE += periph_init_rtc
# Add a routing protocol
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
# This application dumps received packets to STDIO using the pktdump module
USEMODULE += gnrc_pktdump
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Uncomment the following 2 lines to specify static link lokal IPv6 address
# this might be useful for testing, in cases where you cannot or do not want to
# run a shell with ifconfig to get the real link lokal address.
#IPV6_STATIC_LLADDR ?= '"fe80::cafe:cafe:cafe:1"'
#CFLAGS += -DGNRC_IPV6_STATIC_LLADDR=$(IPV6_STATIC_LLADDR)

# Uncomment this to join RPL DODAGs even if DIOs do not contain
# DODAG Configuration Options (see the doc for more info)
# CFLAGS += -DCONFIG_GNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk
# APPLICATION = node
# CFLAGS += -DCONFIG_IEEE802154_DEFAULT_SUBGHZ_CHANNEL=0
# CFLAGS += -DCONFIG_IEEE802154_DEFAULT_SUBGHZ_PAGE=2
# BOARD ?= samr30-xpro
# RIOTBASE ?= $(CURDIR)/../..

# USEMODULE += gnrc_netdev_default
# USEMODULE += gnrc_sixlowpan_router_default

# USEMODULE += gnrc_icmpv6_error#When using `ethos` and `gnrc_uhcpc` with the `gnrc_border_router` example, these steps are applied automatically by either the [host side scripting](https://github.com/RIOT-OS/RIOT/blob/master/dist/tools/ethos/setup_network.sh) or by the [`gnrc_uhcpc.c` code](https://github.com/RIOT-OS/RIOT/blob/master/sys/net/gnrc/application_layer/uhcpc/gnrc_uhcpc.c#L57). Technically nothing obstructs from using this tooling with USB CDC ECM, however the steps are shown here manually with explanation to give the reader insight into the requirements for routing and connectivity
# USEMODULE += gnrc_pktdump
# USEMODULE += gnrc_udp
# USEMODULE += gnrc_rpl
# USEMODULE += gnrc_icmpv6_echo

# USEMODULE += auto_init_gnrc_netif
# USEMODULE += auto_init_gnrc_rpl

# USEMODULE += shell
# USEMODULE += shell_commands
# # Optional but useful
# USEMODULE += ps
# USEMODULE += netstats_l2
# USEMODULE += netstats_ipv6
# USEMODULE += netstats_rpl

# DEVELHELP ?= 1

# include $(RIOTBASE)/Makefile.include
