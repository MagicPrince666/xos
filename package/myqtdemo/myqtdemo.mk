################################################################################
#
# My qt demo
# 846863428@qq.com
# 2021/4/20
#
################################################################################


MYQTDEMO_VERSION = 1.0.0
MYQTDEMO_SITE = $(TOPDIR)/package/myqtdemo/src
MYQTDEMO_SITE_METHOD = local
MYQTDEMO_INSTALL_TARGET = YES
MYQTDEMO_DEPENDENCIES = 
MYQTDEMO_DIR := $(BUILD_DIR)/myqtdemo

define MYQTDEMO_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MYQTDEMO_CLEAN_CMDS
    make -C $(@D) clean
endef

define MYQTDEMO_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/myqtdemo $(TARGET_DIR)/usr/bin
endef

define MYQTDEMO_PERMISSIONS
    /usr/bin/myqtdemo f 4755 0 0 - - - - -
endef

define MYQTDEMO_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
