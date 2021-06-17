################################################################################
#
# git demo test
# huangliquan@xa.com
# 2021/5/10
# http://mirror.yongbok.net/nongnu/fbi-improved/fim-0.6-trunk.tar.gz
################################################################################

FBI_IMPROVED_VERSION = 0.6
FBI_IMPROVED_SOURCE = fim-$(FBI_IMPROVED_VERSION)-trunk.tar.gz
FBI_IMPROVED_SITE = http://mirror.yongbok.net/nongnu/fbi-improved/$(FBI_IMPROVED_SOURCE)

# For the target variant, we only want to build/install libltdl
#FBI_IMPROVED_SUBDIR = fbi-improved

FBI_IMPROVED_INSTALL_STAGING = YES

FBI_IMPROVED_CONF_OPTS = --disable-exif

FBI_IMPROVED_DEPENDENCIES = jpeg libexif libpng tiff libdjvulibre sdl
 
FBI_IMPROVED_LICENSE = LGPL-2.1+
FBI_IMPROVED_LICENSE_FILES = $(FBI_IMPROVED_SUBDIR)/COPYING.LIB

define FBI_IMPROVED_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
FBI_IMPROVED_PRE_CONFIGURE_HOOKS += FBI_IMPROVED_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

