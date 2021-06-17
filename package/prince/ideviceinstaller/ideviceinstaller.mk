################################################################################
#
# ideviceinstaller
# https://github.com/libimobiledevice/ideviceinstaller/releases/download/1.1.1/ideviceinstaller-1.1.1.tar.bz2
#
################################################################################

IDEVICEINSTALLER_VERSION = 1.1.1
IDEVICEINSTALLER_SOURCE = ideviceinstaller-$(IDEVICEINSTALLER_VERSION).tar.bz2
IDEVICEINSTALLER_SITE = https://github.com/libimobiledevice/ideviceinstaller/releases/download/$(IDEVICEINSTALLER_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBIMOBILEDEVICE_SUBDIR = libltdl

IDEVICEINSTALLER_INSTALL_STAGING = YES

IDEVICEINSTALLER_CONF_OPTS = 

IDEVICEINSTALLER_DEPENDENCIES = libplist libimobiledevice libzip

IDEVICEINSTALLER_LICENSE = LGPL-2.1+
IDEVICEINSTALLER_LICENSE_FILES = $(IDEVICEINSTALLER_SUBDIR)/COPYING.LIB

define IDEVICEINSTALLER_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
IDEVICEINSTALLER_PRE_CONFIGURE_HOOKS += IDEVICEINSTALLER_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
IDEVICEINSTALLER = $(HOST_DIR)/bin/ideviceinstaller
