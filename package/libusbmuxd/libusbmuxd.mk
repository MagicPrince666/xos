################################################################################
#
# libusbmuxd
# https://github.com/libimobiledevice/libusbmuxd/releases/download/2.0.2/libusbmuxd-2.0.2.tar.bz2
#
################################################################################

LIBUSBMUXD_VERSION = 2.0.2
LIBUSBMUXD_SOURCE = libusbmuxd-$(LIBUSBMUXD_VERSION).tar.bz2
LIBUSBMUXD_SITE = https://github.com/libimobiledevice/libusbmuxd/releases/download/$(LIBUSBMUXD_VERSION)

LIBUSBMUXD_INSTALL_STAGING = YES

LIBUSBMUXD_CONF_OPTS =

LIBUSBMUXD_DEPENDENCIES = libplist

LIBUSBMUXD_LICENSE = LGPL-2.1
LIBUSBMUXD_LICENSE_FILES = $(LIBUSBMUXD_SUBDIR)/COPYING

define LIBUSBMUXD_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
LIBUSBMUXD_PRE_CONFIGURE_HOOKS += LIBUSBMUXD_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
LIBUSBMUXD = $(HOST_DIR)/bin/libusbmuxd
