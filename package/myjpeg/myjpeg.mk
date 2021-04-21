################################################################################
#
# Display jpeg
# 846863428@qq.com
# 2021/4/20
#
################################################################################


MYJPEG_VERSION = 1.0.0
MYJPEG_SITE = $(TOPDIR)/package/myjpeg/src
MYJPEG_SITE_METHOD = local
MYJPEG_INSTALL_TARGET = YES
MYJPEG_DEPENDENCIES = jpeg
MYJPEG_DIR := $(BUILD_DIR)/myjpeg

define MYJPEG_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MYJPEG_CLEAN_CMDS
    make -C $(@D) clean
endef

define MYJPEG_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/myjpeg $(TARGET_DIR)/usr/bin
endef

define MYJPEG_PERMISSIONS
    /usr/bin/myjpeg f 4755 0 0 - - - - -
endef

define MYJPEG_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
