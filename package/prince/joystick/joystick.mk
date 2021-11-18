################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


JOYSTICK_VERSION = 1.0.0
JOYSTICK_SITE = $(TOPDIR)/package/prince/joystick/src
JOYSTICK_SITE_METHOD = local
JOYSTICK_INSTALL_TARGET = YES
JOYSTICK_DEPENDENCIES = 
JOYSTICK_DIR := $(BUILD_DIR)/joystick

define JOYSTICK_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define JOYSTICK_CLEAN_CMDS
    make -C $(@D) clean
endef

define JOYSTICK_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/joystick $(TARGET_DIR)/usr/bin
endef

define JOYSTICK_PERMISSIONS
    /usr/bin/joystick f 4755 0 0 - - - - -
endef

define JOYSTICK_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
