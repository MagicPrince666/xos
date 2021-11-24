################################################################################
#
# ultrasonic
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


ULTRASONIC_VERSION = 1.0.0
ULTRASONIC_SITE = $(TOPDIR)/package/prince/ultrasonic/src
ULTRASONIC_SITE_METHOD = local
ULTRASONIC_INSTALL_TARGET = YES
ULTRASONIC_DEPENDENCIES = 
ULTRASONIC_DIR := $(BUILD_DIR)/ultrasonic

define ULTRASONIC_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define ULTRASONIC_CLEAN_CMDS
    make -C $(@D) clean
endef

define ULTRASONIC_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/ultrasonic $(TARGET_DIR)/usr/bin
endef

define ULTRASONIC_PERMISSIONS
    /usr/bin/ultrasonic f 4755 0 0 - - - - -
endef

define ULTRASONIC_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
