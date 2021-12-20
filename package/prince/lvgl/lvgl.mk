################################################################################
#
# lvgl
# 846863428@qq.com
# 2021/10/20
#
################################################################################

#仓库分支名称
LVGL_VERSION = master
#仓库git地址
LVGL_SITE = https://github.com/lvgl/lv_port_linux_frame_buffer
#LVGL_SITE = $(call github,lvgl,lv_port_linux_frame_buffer,$(LVGL_VERSION))
#获取源码的方式
LVGL_SITE_METHOD = git
LVGL_SOURCE = lv_port_linux_frame_buffer.tar.gz

LVGL_ALWAYS_BUILD = YES
LVGL_INSTALL_STAGING = YES
LVGL_GIT_SUBMODULES = YES
LVGL_CFLAGS =
LVGL_LDFLAGS =

LVGL_DEPENDENCIES = tslib

LVGL_LICENSE = MIT
LVGL_LICENSE_FILES = LICENSE

define LVGL_BUILD_CMDS
    $(TARGET_MAKE_ENV) make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define LVGL_CLEAN_CMDS
    $(TARGET_MAKE_ENV) make -C $(@D) clean
endef

define LVGL_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/demo $(TARGET_DIR)/usr/bin
endef

define LVGL_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
