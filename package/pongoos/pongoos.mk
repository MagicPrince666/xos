################################################################################
#
# pongoos
# 846863428@qq.com
# 2021/9/8
#
################################################################################

#仓库分支名称
PONGOOS_VERSION = master
#仓库git地址
PONGOOS_SITE = https://github.com/checkra1n/pongoOS
#获取源码的方式
PONGOOS_SITE_METHOD = git
PONGOOS_SOURCE = pongoOS.tar.gz
PONGOOS_ALWAYS_BUILD = YES
PONGOOS_INSTALL_STAGING = YES
PONGOOS_CFLAGS =
PONGOOS_LDFLAGS =

define PONGOOS_BUILD_CMDS
    $(TARGET_MAKE_ENV) make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define PONGOOS_CLEAN_CMDS
    $(TARGET_MAKE_ENV) make -C $(@D) clean
endef

define PONGOOS_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/checkra1n $(TARGET_DIR)/usr/bin
endef

define PONGOOS_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
