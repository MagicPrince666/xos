################################################################################
#
# inv_mpu
# huangliquan@xa.com
# 2020/10/9
#
################################################################################


INV_MPU_VERSION = 1.0.0
INV_MPU_SITE = $(TOPDIR)/package/prince/inv_mpu/src
INV_MPU_SITE_METHOD = local
INV_MPU_INSTALL_TARGET = YES
INV_MPU_DEPENDENCIES = 
INV_MPU_DIR := $(BUILD_DIR)/inv_mpu

define INV_MPU_BUILD_CMDS
    make CC=$(TARGET_CC) CXX=$(TARGET_CXX) -C $(@D)
endef

define INV_MPU_CLEAN_CMDS
    make -C $(@D) clean
endef

define INV_MPU_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/inv_mpu $(TARGET_DIR)/usr/bin
endef

define INV_MPU_PERMISSIONS
    /usr/bin/inv_mpu f 4755 0 0 - - - - -
endef

define INV_MPU_UNINSTALL_TARGET_CMDS
    $(TARGET_MAKE_ENV) $(MAKE) -C $(@D) uninstall
endef

$(eval $(generic-package))
