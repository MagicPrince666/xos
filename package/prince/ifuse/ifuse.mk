################################################################################
#
# ifuse
# https://github.com/libimobiledevice/ifuse/releases/download/1.1.4/ifuse-1.1.4.tar.bz2
#
################################################################################

IFUSE_VERSION = 1.1.4
IFUSE_SOURCE = ifuse-$(IFUSE_VERSION).tar.bz2
IFUSE_SITE = https://github.com/libimobiledevice/ifuse/releases/download/$(IFUSE_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBIMOBILEDEVICE_SUBDIR = libltdl

IFUSE_INSTALL_STAGING = YES

IFUSE_CONF_OPTS = 

IFUSE_DEPENDENCIES = libfuse libplist libimobiledevice

IFUSE_LICENSE = LGPL-2.1+
IFUSE_LICENSE_FILES = $(IFUSE_SUBDIR)/COPYING.LIB

define IFUSE_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
IFUSE_PRE_CONFIGURE_HOOKS += IFUSE_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
IFUSE = $(HOST_DIR)/bin/ifuse
