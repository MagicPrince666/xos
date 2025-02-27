################################################################################
#
# MOONRAKER https://github.com/Arksine/moonraker.git
#
################################################################################

# TODO: add config to create default user if does not exists
MOONRAKER_VERSION = v0.7.1
MOONRAKER_SITE = $(call github,Arksine,moonraker,$(MOONRAKER_VERSION))
MOONRAKER_DEPENDENCIES = klipper3d python-prompt-toolkit python-pygments
MOONRAKER_SETUP_TYPE = setuptools

define MOONRAKER_BUILD_CMDS
	cp $(MOONRAKER_PKGDIR)/pyproject.toml $(@D)/pyproject.toml		
endef

define MOONRAKER_INSTALL_TARGET_CMDS
	mkdir -p -m 0755 $(TARGET_DIR)/opt/moonraker
	cp -rf $(@D)/moonraker $(@D)/scripts $(TARGET_DIR)/opt/moonraker
	cp $(MOONRAKER_PKGDIR)/moonraker.conf  $(TARGET_DIR)/opt/moonraker/moonraker.cfg
endef

# Custom SYSV init script
# see https://github.com/buildroot/buildroot/blob/master/package/restorecond/S02restorecond
define MOONRAKER_INSTALL_INIT_SYSV
	$(INSTALL) -m 0755 -D $(MOONRAKER_PKGDIR)/S91moonraker $(TARGET_DIR)/etc/init.d
endef

define MOONRAKER_INSTALL_INIT_SYSTEMD
	$(INSTALL) -D -m 0644 $(MOONRAKER_PKGDIR)/moonraker.service \
		$(TARGET_DIR)/usr/lib/systemd/system/moonraker.service
endef

define MOONRAKER_USERS
	moonraker -1 moonraker -1 * - - - Moonraker daemon
endef

$(eval $(python-package))