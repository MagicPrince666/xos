################################################################################
#
# https://github.com/libimobiledevice/libimobiledevice-glue.git
#
################################################################################
LIBIMOBILEDEVICE_GLUE_VERSION = master
LIBIMOBILEDEVICE_GLUE_SITE = $(call github,libimobiledevice,libimobiledevice-glue,$(LIBIMOBILEDEVICE_GLUE_VERSION))
LIBIMOBILEDEVICE_GLUE_INSTALL_TARGET = YES
LIBIMOBILEDEVICE_GLUE_INSTALL_STAGING = YES

LIBIMOBILEDEVICE_GLUE_CONF_OPTS = --disable-static
LIBIMOBILEDEVICE_GLUE_DEPENDENCIES = libplist host-libtool host-autoconf host-automake

LIBIMOBILEDEVICE_GLUE_LICENSE = LGPL-2.1
LIBIMOBILEDEVICE_GLUE_LICENSE_FILES = $(LIBIMOBILEDEVICE_GLUE_SUBDIR)/COPYING

define LIBIMOBILEDEVICE_GLUE_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
    cd $(@D) && ./autogen.sh --prefix=$(HOST_DIR)/usr/lib --host=$(GNU_TARGET_NAME) CC=$(TARGET_CC) CXX=$(TARGET_CXX)
endef
LIBIMOBILEDEVICE_GLUE_PRE_CONFIGURE_HOOKS += LIBIMOBILEDEVICE_GLUE_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

# variables used by other packages
LIBIMOBILEDEVICE_GLUE = $(HOST_DIR)/bin/libimobiledevice-gule