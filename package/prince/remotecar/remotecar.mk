################################################################################
#
# linux remote car
#
################################################################################
REMOTECAR_VERSION = master
REMOTECAR_SITE = $(call github,MagicPrince666,f1c100scar,$(REMOTECAR_VERSION))
REMOTECAR_DEPENDENCIES = 
REMOTECAR_INSTALL_STAGING = YES
REMOTECAR_INSTALL_TARGET = YES

REMOTECAR_CONF_OPTS =

REMOTECAR_DIR := $(BUILD_DIR)/RemoteCar

define REMOTECAR_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define REMOTECAR_CLEAN_CMDS
    make -C $(@D) clean
endef

define REMOTECAR_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/RemoteCar $(TARGET_DIR)/usr/sbin
    $(INSTALL) -D -m 0755 $(@D)/S99remotecar $(TARGET_DIR)/etc/init.d
endef

define REMOTECAR_PERMISSIONS
    /usr/sbin/RemoteCar f 4755 0 0 - - - - -
endef

define REMOTECAR_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
