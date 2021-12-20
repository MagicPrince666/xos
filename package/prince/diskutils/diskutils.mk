################################################################################
#
# WL apple disk utils
#
################################################################################
WLUTILS_VERSION = main
WLUTILS_SITE = $(call github,MagicPrince666,uart,$(WLUTILS_VERSION))
WLUTILS_DEPENDENCIES = 
WLUTILS_INSTALL_STAGING = YES
WLUTILS_INSTALL_TARGET = YES

WLUTILS_CONF_OPTS =

WLUTILS_DIR := $(BUILD_DIR)/wlutils

#define WLUTILS_BUILD_CMDS
#    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
#endef

#define WLUTILS_CLEAN_CMDS
#    make -C $(@D) clean
#endef

#define WLUTILS_INSTALL_TARGET_CMDS
#    $(INSTALL) -D -m 0755 $(@D)/wlutils $(TARGET_DIR)/usr/bin
#endef

#define WLUTILS_PERMISSIONS
#    /usr/bin/wlutils f 4755 0 0 - - - - -
#endef

define WLUTILS_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(cmake-package))
