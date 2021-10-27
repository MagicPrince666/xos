################################################################################
#
# mp4 player
# 846863428@qq.com
# 2021/10/22
#
################################################################################


MYFFMPEG_VERSION = 1.0.0
MYFFMPEG_SITE = $(TOPDIR)/package/prince/myffmpeg/src
MYFFMPEG_SITE_METHOD = local
MYFFMPEG_INSTALL_TARGET = YES
MYFFMPEG_DEPENDENCIES = ffmpeg sdl2
MYFFMPEG_DIR := $(BUILD_DIR)/myffmpeg

define MYFFMPEG_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MYFFMPEG_CLEAN_CMDS
    make -C $(@D) clean
endef

define MYFFMPEG_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/mp4play $(TARGET_DIR)/usr/bin
#    $(INSTALL) -D -m 0755 $(@D)/test.mp4 $(TARGET_DIR)/root
endef

define MYFFMPEG_PERMISSIONS
    /usr/bin/mp4play f 4755 0 0 - - - - -
endef

define MYFFMPEG_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
