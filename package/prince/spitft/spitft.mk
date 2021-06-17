################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


SPITFT_VERSION = 1.0.0
SPITFT_SITE = $(TOPDIR)/package/prince/spitft/src
SPITFT_SITE_METHOD = local
SPITFT_INSTALL_TARGET = YES
SPITFT_DEPENDENCIES = 
SPITFT_DIR := $(BUILD_DIR)/spitft

define SPITFT_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define SPITFT_CLEAN_CMDS
    make -C $(@D) clean
endef

define SPITFT_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/spitft $(TARGET_DIR)/usr/bin
endef

define SPITFT_PERMISSIONS
    /usr/bin/spitft f 4755 0 0 - - - - -
endef

define SPITFT_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
