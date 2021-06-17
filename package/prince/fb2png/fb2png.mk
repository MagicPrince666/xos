################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


FB2PNG_VERSION = 1.0.0
FB2PNG_SITE = $(TOPDIR)/package/prince/fb2png/src
FB2PNG_SITE_METHOD = local
FB2PNG_INSTALL_TARGET = YES
FB2PNG_DEPENDENCIES = 
FB2PNG_DIR := $(BUILD_DIR)/fb2png

define FB2PNG_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define FB2PNG_CLEAN_CMDS
    make -C $(@D) clean
endef

define FB2PNG_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/fb2png $(TARGET_DIR)/usr/bin
endef

define FB2PNG_PERMISSIONS
    /usr/bin/fb2png f 4755 0 0 - - - - -
endef

define FB2PNG_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
