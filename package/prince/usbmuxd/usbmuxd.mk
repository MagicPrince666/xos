################################################################################
#
# usbmuxd
# https://github.com/libimobiledevice/usbmuxd/releases/download/1.1.1/usbmuxd-1.1.1.tar.bz2
#
################################################################################

USBMUXD_VERSION = 1.1.1
USBMUXD_SOURCE = usbmuxd-$(USBMUXD_VERSION).tar.bz2
USBMUXD_SITE = https://github.com/libimobiledevice/usbmuxd/releases/download/$(USBMUXD_VERSION)

# For the target variant, we only want to build/install libltdl
#USBMUXD_SUBDIR = libltdl
HOST_USBMUXD_SUBDIR = .

USBMUXD_INSTALL_STAGING = YES

USBMUXD_CONF_OPTS =

USBMUXD_DEPENDENCIES = libusbmuxd

USBMUXD_LICENSE = GPLv3
USBMUXD_LICENSE_FILES = $(USBMUXD_SUBDIR)/COPYING
HOST_USBMUXD_LICENSE = GPLv3 (usbmuxd)
HOST_USBMUXD_LICENSE_FILES = COPYING $(USBMUXD_SUBDIR)/COPYING.LIB

HOST_USBMUXD_CONF_ENV = MAKEINFO=true
HOST_USBMUXD_USBMUXD_PATCH = NO

define USBMUXD_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
USBMUXD_PRE_CONFIGURE_HOOKS += USBMUXD_AVOID_AUTORECONF_HOOK
HOST_USBMUXD_PRE_CONFIGURE_HOOKS += USBMUXD_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
USBMUXD = $(HOST_DIR)/usr/bin/usbmuxd
