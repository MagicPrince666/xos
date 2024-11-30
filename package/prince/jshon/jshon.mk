################################################################################
#
# jshon
#
################################################################################
JSHON_VERSION = master
JSHON_SITE = $(call github,keenerd,jshon,$(JSHON_VERSION))
JSHON_DEPENDENCIES = 
JSHON_INSTALL_STAGING = YES
JSHON_INSTALL_TARGET = YES

JSHON_LICENSE = MIT
JSHON_LICENSE_FILES = COPYING

define JSHON_BUILD_CMDS
$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define JSHON_INSTALL_TARGET_CMDS
$(INSTALL) -D -m 0755 $(@D)/jshon $(TARGET_DIR)/usr/bin/jshon
endef

$(eval $(generic-package))