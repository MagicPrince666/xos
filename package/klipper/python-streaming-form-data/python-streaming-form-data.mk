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
PYTHON_STREAMING_FORM_DATA_DEPENDENCIES = python-prompt-toolkit python-pygments

define PYTHON_STREAMING_FORM_DATA_BUILD_CMDS
	cp $(PYTHON_STREAMING_FORM_DATA_PKGDIR)/setup.py $(@D)/setup.py	
endef

$(eval $(python-package))
