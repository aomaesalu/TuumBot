ifdef APP
USRSRC := applications/$(APP)
endif

include $(PROJECT_ROOT)/user/user.mk

include $(PROJECT_ROOT)/user/$(USRSRC)/import.mk #$(call rwildcard,$(PROJECT_ROOT)/user/$(USRSRC)/,import.mk)
