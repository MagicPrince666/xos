################################################################################
#
# python-streaming-form-data
#
################################################################################

PYTHON_STREAMING_FORM_DATA_VERSION = v1.19.1
PYTHON_STREAMING_FORM_DATA_SITE = $(call github,siddhantgoel,streaming-form-data,$(PYTHON_STREAMING_FORM_DATA_VERSION))
PYTHON_STREAMING_FORM_DATA_LICENSE = MIT
PYTHON_STREAMING_FORM_DATA_LICENSE_FILES = LICENSE.txt
PYTHON_STREAMING_FORM_DATA_SETUP_TYPE = setuptools
PYTHON_STREAMING_FORM_DATA_DEPENDENCIES = python3

define PYTHON_STREAMING_FORM_DATA_BUILD_CMDS
	cp $(PYTHON_STREAMING_FORM_DATA_PKGDIR)/setup.py $(@D)/setup.py	
	$(TARGET_MAKE_ENV) $(TARGET_CC) $(@D)/src/streaming_form_data/_parser.c -I$(STAGING_DIR)/usr/include/python$(PYTHON3_VERSION_MAJOR) -shared -o $(@D)/libstreaming_form_data.so
endef

define PYTHON_STREAMING_FORM_DATA_INSTALL_TARGET_CMDS
	cp -rf $(@D)/libstreaming_form_data.so $(TARGET_DIR)/usr/lib/libstreaming_form_data.so
endef


$(eval $(python-package))
