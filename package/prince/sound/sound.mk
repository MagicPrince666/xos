################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


SOUND_PLAY_VERSION = 1.0.0
SOUND_PLAY_SITE = $(TOPDIR)/package/prince/sound/src
SOUND_PLAY_SITE_METHOD = local
SOUND_PLAY_INSTALL_TARGET = YES
SOUND_PLAY_DEPENDENCIES = alsa-lib
SOUND_PLAY_DIR := $(BUILD_DIR)/sound

define SOUND_PLAY_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define SOUND_PLAY_CLEAN_CMDS
    make -C $(@D) clean
endef

define SOUND_PLAY_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/sound $(TARGET_DIR)/usr/bin
endef

define SOUND_PLAY_PERMISSIONS
    /usr/bin/sound f 4755 0 0 - - - - -
endef

define SOUND_PLAY_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
