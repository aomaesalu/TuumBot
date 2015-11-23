MODULE=rtex

PROJECT_ROOT=../../..
RTEX_MODULE_PATH=.

BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)
HAL_LINK :=

DEPENDENCIES = user #hal system vision loc entities geometry rtxmath
MAKE_DEPENDENCIES = user# hal system vision loc entities geometry rtxmath

include ../modular.mk
include $(PROJECT_ROOT)/build/platform-id.mk

TARGET=out elf


include $(PROJECT_ROOT)/build/tlm.mk
