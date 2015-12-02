LOC_MODULE_PATH ?= $(PROJECT_ROOT)/loc
include $(call rwildcard,$(LOC_MODULE_PATH)/,include.mk)

LOC_LIB_DIR = $(BUILD_PATH_BASE)/loc/$(BUILD_TARGET_PLATFORM)
LOC_LIB_DEP = $(LOC_LIB_DIR)/libloc.a
