################################################################################
#
# libirecovery
# https://github.com/libimobiledevice/libirecovery/releases/download/1.0.0/libirecovery-1.0.0.tar.bz2
#
################################################################################

LIBIRECOVERY_VERSION = 1.0.0
LIBIRECOVERY_SOURCE = libirecovery-$(LIBIRECOVERY_VERSION).tar.bz2
LIBIRECOVERY_SITE = https://github.com/libimobiledevice/libirecovery/releases/download/$(LIBIRECOVERY_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBIMOBILEDEVICE_SUBDIR = libltdl

LIBIRECOVERY_INSTALL_STAGING = YES

LIBIRECOVERY_CONF_OPTS = 

LIBIRECOVERY_DEPENDENCIES = libusb

LIBIRECOVERY_LICENSE = LGPL-2.1+
LIBIRECOVERY_LICENSE_FILES = $(LIBIRECOVERY_SUBDIR)/COPYING.LIB

define LIBIRECOVERY_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
LIBIRECOVERY_PRE_CONFIGURE_HOOKS += LIBIRECOVERY_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
LIBIRECOVERY = $(HOST_DIR)/bin/irecovery
