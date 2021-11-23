################################################################################
#
# xag imu test
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


SOUND_VERSION = 1.0.0
SOUND_SITE = $(TOPDIR)/package/prince/sound/src
SOUND_SITE_METHOD = local
SOUND_INSTALL_TARGET = YES
SOUND_DEPENDENCIES = alsa-lib libmad libid3tag
SOUND_DIR := $(BUILD_DIR)/sound

define SOUND_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define SOUND_CLEAN_CMDS
    make -C $(@D) clean
endef

define SOUND_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/sound $(TARGET_DIR)/usr/bin
endef

define SOUND_PERMISSIONS
    /usr/bin/sound f 4755 0 0 - - - - -
endef

define SOUND_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
