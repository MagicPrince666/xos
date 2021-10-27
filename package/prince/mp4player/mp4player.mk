################################################################################
#
# mp4 player
# 846863428@qq.com
# 2021/10/22
#
################################################################################


MP4PLAYER_VERSION = 1.0.0
MP4PLAYER_SITE = $(TOPDIR)/package/prince/mp4player/src
MP4PLAYER_SITE_METHOD = local
MP4PLAYER_INSTALL_TARGET = YES
MP4PLAYER_DEPENDENCIES = ffmpeg sdl2
MP4PLAYER_DIR := $(BUILD_DIR)/mp4player

define MP4PLAYER_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MP4PLAYER_CLEAN_CMDS
    make -C $(@D) clean
endef

define MP4PLAYER_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/mp4player $(TARGET_DIR)/usr/bin
    $(INSTALL) -D -m 0755 $(@D)/bigbuckbunny_320x240.h264 $(TARGET_DIR)/root
endef

define MP4PLAYER_PERMISSIONS
    /usr/bin/mp4player f 4755 0 0 - - - - -
endef

define MP4PLAYER_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
