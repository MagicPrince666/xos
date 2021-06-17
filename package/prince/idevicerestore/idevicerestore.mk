################################################################################
#
# idevicerestore
# https://github.com/libimobiledevice/idevicerestore/releases/download/1.0.0/idevicerestore-1.0.0.tar.bz2
#
################################################################################

IDEVICERESTORE_VERSION = 1.0.0
IDEVICERESTORE_SOURCE = idevicerestore-$(IDEVICERESTORE_VERSION).tar.bz2
IDEVICERESTORE_SITE = https://github.com/libimobiledevice/idevicerestore/releases/download/$(IDEVICERESTORE_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBIMOBILEDEVICE_SUBDIR = libltdl

IDEVICERESTORE_INSTALL_STAGING = YES

IDEVICERESTORE_CONF_OPTS = 

IDEVICERESTORE_DEPENDENCIES = libplist libusbmuxd libopenssl readline libimobiledevice libirecovery libzip libcurl

IDEVICERESTORE_LICENSE = LGPL-2.1+
IDEVICERESTORE_LICENSE_FILES = $(IDEVICERESTORE_SUBDIR)/COPYING.LIB


define IDEVICERESTORE_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
IDEVICERESTORE_PRE_CONFIGURE_HOOKS += IDEVICERESTORE_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
IDEVICERESTORE = $(HOST_DIR)/bin/libimobiledevice
