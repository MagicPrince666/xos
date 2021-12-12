################################################################################
#
# touchscreen
#
################################################################################
TOUCHSCREEN_VERSION = main
TOUCHSCREEN_SITE = $(call github,MagicPrince666,touchscreen,$(TOUCHSCREEN_VERSION))
TOUCHSCREEN_DEPENDENCIES = 
TOUCHSCREEN_INSTALL_STAGING = YES
TOUCHSCREEN_INSTALL_TARGET = YES

TOUCHSCREEN_CONF_OPTS =

TOUCHSCREEN_DIR := $(BUILD_DIR)/touchscreen

define TOUCHSCREEN_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define TOUCHSCREEN_CLEAN_CMDS
    make -C $(@D) clean
endef

define TOUCHSCREEN_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/touchscreen $(TARGET_DIR)/usr/bin
endef

define TOUCHSCREEN_PERMISSIONS
    /usr/bin/touchscreen f 4755 0 0 - - - - -
endef

define TOUCHSCREEN_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
