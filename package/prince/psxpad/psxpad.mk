################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


PSXPAD_VERSION = 1.0.0
PSXPAD_SITE = $(TOPDIR)/package/prince/psxpad/src
PSXPAD_SITE_METHOD = local
PSXPAD_INSTALL_TARGET = YES
PSXPAD_DEPENDENCIES = 
PSXPAD_DIR := $(BUILD_DIR)/psxpad

define PSXPAD_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define PSXPAD_CLEAN_CMDS
    make -C $(@D) clean
endef

define PSXPAD_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/psxpad $(TARGET_DIR)/usr/bin
endef

define PSXPAD_PERMISSIONS
    /usr/bin/psxpad f 4755 0 0 - - - - -
endef

define PSXPAD_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
