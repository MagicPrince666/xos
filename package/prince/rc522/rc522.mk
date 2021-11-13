################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


RC522_VERSION = 1.0.0
RC522_SITE = $(TOPDIR)/package/prince/rc522/src
RC522_SITE_METHOD = local
RC522_INSTALL_TARGET = YES
RC522_DEPENDENCIES = 
RC522_DIR := $(BUILD_DIR)/rc522

define RC522_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define RC522_CLEAN_CMDS
    make -C $(@D) clean
endef

define RC522_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/rc522 $(TARGET_DIR)/usr/bin
endef

define RC522_PERMISSIONS
    /usr/bin/rc522 f 4755 0 0 - - - - -
endef

define RC522_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
