################################################################################
#
# My opencv demo
# 846863428@qq.com
# 2021/4/20
#
################################################################################


MYOPENCV_VERSION = 1.0.0
MYOPENCV_SITE = $(TOPDIR)/package/prince/myopencv/src
MYOPENCV_SITE_METHOD = local
MYOPENCV_INSTALL_TARGET = YES
MYOPENCV_DEPENDENCIES = opencv3 jpeg
MYOPENCV_DIR := $(BUILD_DIR)/myopencv

define MYOPENCV_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MYOPENCV_CLEAN_CMDS
    make -C $(@D) clean
endef

define MYOPENCV_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/myopencv $(TARGET_DIR)/usr/bin
endef

define MYOPENCV_PERMISSIONS
    /usr/bin/myopencv f 4755 0 0 - - - - -
endef

define MYOPENCV_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
