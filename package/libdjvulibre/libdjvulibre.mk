################################################################################
#
# libdjvulibre
# https://sourceforge.net/projects/djvu/files/DjVuLibre/3.5.28/djvulibre-3.5.28.tar.gz
#
################################################################################

LIBDJVULIBRE_VERSION = 3.5.28
LIBDJVULIBRE_SOURCE = djvulibre-$(LIBDJVULIBRE_VERSION).tar.gz
LIBDJVULIBRE_SITE = https://sourceforge.net/projects/djvu/files/DjVuLibre/$(LIBDJVULIBRE_VERSION)

# For the target variant, we only want to build/install libltdl
#LIBDJVULIBRE_SUBDIR = libltdl

LIBDJVULIBRE_INSTALL_STAGING = YES

LIBDJVULIBRE_CONF_OPTS = 

LIBDJVULIBRE_DEPENDENCIES = 

LIBDJVULIBRE_LICENSE = GPLV2
LIBDJVULIBRE_LICENSE_FILES = $(LIBDJVULIBRE_SUBDIR)/COPYING.LIB

define LIBDJVULIBRE_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
LIBDJVULIBRE_PRE_CONFIGURE_HOOKS += LIBDJVULIBRE_AVOID_AUTORECONF_HOOK

$(eval $(autotools-package))

