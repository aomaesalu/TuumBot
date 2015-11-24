RTEX_MODULE_VERSION ?= 0

ifeq ("$(SYSTEM_MODULE_NAME)","")
include $(PROJECT_ROOT)/modules/tuum/rtex/rtex.mk
endif

DEPS_IMPORT_SCRIPTS =$(foreach module,$($(MODULE)_DEPENDENCIES),$(PROJECT_ROOT)/$(module)/import.mk)
include $(DEPS_IMPORT_SCRIPTS)

include $(call rwildcard,$(RTEX_MODULE_PATH)/,include.mk)
