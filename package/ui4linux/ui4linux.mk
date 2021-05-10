################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


UI4LINUX_VERSION = 1.0.0
UI4LINUX_SITE = $(TOPDIR)/package/ui4linux/src
UI4LINUX_SITE_METHOD = local
UI4LINUX_INSTALL_TARGET = YES
UI4LINUX_DEPENDENCIES = libpng
UI4LINUX_DIR := $(BUILD_DIR)/ui4linux

define UI4LINUX_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define UI4LINUX_CLEAN_CMDS
    make -C $(@D) clean
endef

define UI4LINUX_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/ui4linux $(TARGET_DIR)/usr/bin
endef

define UI4LINUX_PERMISSIONS
    /usr/bin/ui4linux f 4755 0 0 - - - - -
endef

define UI4LINUX_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
