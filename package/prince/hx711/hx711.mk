################################################################################
#
# hx711
#
################################################################################
HX711_VERSION = master
HX711_SITE = $(call github,MagicPrince666,hx711,$(HX711_VERSION))
HX711_DEPENDENCIES = 
HX711_INSTALL_STAGING = YES
HX711_INSTALL_TARGET = YES

HX711_CONF_OPTS =

HX711_DIR := $(BUILD_DIR)/hx711

define HX711_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define HX711_CLEAN_CMDS
    make -C $(@D) clean
endef

define HX711_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/hx711 $(TARGET_DIR)/usr/bin
endef

define HX711_PERMISSIONS
    /usr/bin/hx711 f 4755 0 0 - - - - -
endef

define HX711_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
