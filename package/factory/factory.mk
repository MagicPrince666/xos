################################################################################
#
# Factory
# 846863428@qq.com
# 2021/6/15
#
################################################################################


FACTORY_VERSION = 1.0.0
FACTORY_SITE = $(TOPDIR)/package/factory/src
FACTORY_SITE_METHOD = local
FACTORY_INSTALL_TARGET = YES
FACTORY_DEPENDENCIES =
FACTORY_DIR := $(BUILD_DIR)/factory

define FACTORY_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define FACTORY_CLEAN_CMDS
    make -C $(@D) clean
endef

define FACTORY_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/factory $(TARGET_DIR)/usr/bin
endef

define FACTORY_PERMISSIONS
    /usr/bin/factory f 4755 0 0 - - - - -
endef

define FACTORY_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(cmake-package))
