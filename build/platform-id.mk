
# defines
# PLATFORM_NAME - a unique name for the platform, can be used to organise sources
#                 by platform
# PRODUCT_DESC  - text description of the product ID

ifneq (,$(PLATFORM))

ifeq ("$(PLATFORM)","phobos")
PLATFORM_ID = 0
endif

ifeq (,$(PLATFORM_ID))
$(error "Unknown platform: $(PLATFORM))
endif

endif

ifndef PLATFORM_ID
PLATFORM_ID=0
endif

# Determine which is the target device

ARCH=gcc

ifeq ("$(PLATFORM_ID)","0")
PLATFORM=phobos
PLATFORM_NAME=phobos
PRODUCT_DESC=Tuum Phobos (NUC) Robot
DEFAULT_PRODUCT_ID=0
endif
