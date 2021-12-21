################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


WL8PUART_VERSION = 1.0.0
WL8PUART_SITE = $(TOPDIR)/package/prince/wl8puart/src
WL8PUART_SITE_METHOD = local
WL8PUART_INSTALL_TARGET = YES
WL8PUART_DEPENDENCIES = 
WL8PUART_DIR := $(BUILD_DIR)/wl8puart

define WL8PUART_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define WL8PUART_CLEAN_CMDS
    make -C $(@D) clean
endef

define WL8PUART_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/wl8puart $(TARGET_DIR)/usr/bin
endef

define WL8PUART_PERMISSIONS
    /usr/bin/wl8puart f 4755 0 0 - - - - -
endef

define WL8PUART_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
