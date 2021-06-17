################################################################################
#
# libideviceactivation
# https://github.com/libimobiledevice/libideviceactivation/releases/download/1.1.1/libideviceactivation-1.1.1.tar.bz2
#
################################################################################

LIBIDEVICEACTIVATION_VERSION = 1.1.1
LIBIDEVICEACTIVATION_SOURCE = libideviceactivation-$(LIBIDEVICEACTIVATION_VERSION).tar.bz2
LIBIDEVICEACTIVATION_SITE = https://github.com/libimobiledevice/libideviceactivation/releases/download/$(LIBIDEVICEACTIVATION_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBIMOBILEDEVICE_SUBDIR = libltdl

LIBIDEVICEACTIVATION_INSTALL_STAGING = YES

LIBIDEVICEACTIVATION_CONF_OPTS = 

LIBIDEVICEACTIVATION_DEPENDENCIES = libcurl libxml2 libplist libusbmuxd libopenssl libimobiledevice

LIBIDEVICEACTIVATION_LICENSE = LGPL-2.1+
LIBIDEVICEACTIVATION_LICENSE_FILES = $(LIBIDEVICEACTIVATION_SUBDIR)/COPYING.LIB

define LIBIDEVICEACTIVATION_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
LIBIDEVICEACTIVATION_PRE_CONFIGURE_HOOKS += LIBIDEVICEACTIVATION_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
LIBIDEVICEACTIVATION = $(HOST_DIR)/bin/ideviceactivation
