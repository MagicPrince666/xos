################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


SONNYPS2_VERSION = 1.0.0
SONNYPS2_SITE = $(TOPDIR)/package/prince/sonnyps2/src
SONNYPS2_SITE_METHOD = local
SONNYPS2_INSTALL_TARGET = YES
SONNYPS2_DEPENDENCIES = 
SONNYPS2_DIR := $(BUILD_DIR)/sonnyps2

define SONNYPS2_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define SONNYPS2_CLEAN_CMDS
    make -C $(@D) clean
endef

define SONNYPS2_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/sonnyps2 $(TARGET_DIR)/usr/bin
endef

define SONNYPS2_PERMISSIONS
    /usr/bin/sonnyps2 f 4755 0 0 - - - - -
endef

define SONNYPS2_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
