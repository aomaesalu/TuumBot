ifdef APP
USRSRC := applications/$(APP)
endif

include $(PROJECT_ROOT)/user/user.mk

LIB_DEPS +=

include $(call rwildcard,$(USER_MODULE_PATH)/,include.mk)
