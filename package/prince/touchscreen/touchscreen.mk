################################################################################
#
# touchscreen
#
################################################################################
TOUCH_SCREEN_VERSION = main
TOUCH_SCREEN_SITE = $(call github,MagicPrince666,touchscreen,$(TOUCH_SCREEN_VERSION))
TOUCH_SCREEN_DEPENDENCIES = 
TOUCH_SCREEN_INSTALL_STAGING = YES
TOUCH_SCREEN_INSTALL_TARGET = YES

TOUCH_SCREEN_CONF_OPTS =

TOUCH_SCREEN_DIR := $(BUILD_DIR)/touchscreen

define TOUCH_SCREEN_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/touchscreen $(TARGET_DIR)/usr/bin
endef

define TOUCH_SCREEN_PERMISSIONS
    /usr/bin/touchscreen f 4755 0 0 - - - - -
endef

define TOUCH_SCREEN_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(cmake-package))
