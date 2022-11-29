################################################################################
#
# libshml
# https://github.com/SFML/SFML/archive/refs/tags/2.5.1.tar.gz
#
################################################################################

LIBSFML_VERSION = 2.5.1
LIBSFML_SOURCE = $(LIBSFML_VERSION).tar.gz
LIBSFML_SITE = https://github.com/SFML/SFML/archive/refs/tags/$(LIBSFML_VERSION)

# For the target variant, we only want to build/install libsfml
#LIBSFML_SUBDIR = libsfml

LIBSFML_INSTALL_STAGING = YES

LIBSFML_CONF_OPTS = 

LIBSFML_DEPENDENCIES = sdl2

LIBSFML_LICENSE = GPL
LIBSFML_LICENSE_FILES = $(LIBSFML_SUBDIR)/license.md

define LIBSFML_AVOID_AUTORECONF_HOOK
	find $(@D) -name aclocal.m4 -exec touch '{}' \;
	find $(@D) -name config-h.in -exec touch '{}' \;
	find $(@D) -name configure -exec touch '{}' \;
	find $(@D) -name Makefile.in -exec touch '{}' \;
endef
LIBSFML_PRE_CONFIGURE_HOOKS += LIBSFML_AVOID_AUTORECONF_HOOK

$(eval $(cmake-package))

