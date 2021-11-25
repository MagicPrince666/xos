################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


MP3PLAY_VERSION = 1.0.0
MP3PLAY_SITE = $(TOPDIR)/package/prince/mp3play/src
MP3PLAY_SITE_METHOD = local
MP3PLAY_INSTALL_TARGET = YES
MP3PLAY_DEPENDENCIES = alsa-lib mpg123
MP3PLAY_DIR := $(BUILD_DIR)/mp3play

define MP3PLAY_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define MP3PLAY_CLEAN_CMDS
    make -C $(@D) clean
endef

define MP3PLAY_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/mp3play $(TARGET_DIR)/usr/bin
endef

define MP3PLAY_PERMISSIONS
    /usr/bin/mp3play f 4755 0 0 - - - - -
endef

define MP3PLAY_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
