################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


IR_REMOTE_VERSION = 1.0.0
IR_REMOTE_SITE = $(TOPDIR)/package/prince/ir-remote/src
IR_REMOTE_SITE_METHOD = local
IR_REMOTE_INSTALL_TARGET = YES
IR_REMOTE_DEPENDENCIES = 
IR_REMOTE_DIR := $(BUILD_DIR)/ir-remote

define IR_REMOTE_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define IR_REMOTE_CLEAN_CMDS
    make -C $(@D) clean
endef

define IR_REMOTE_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/ir-remote $(TARGET_DIR)/usr/bin
endef

define IR_REMOTE_PERMISSIONS
    /usr/bin/ir-remote f 4755 0 0 - - - - -
endef

define IR_REMOTE_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
