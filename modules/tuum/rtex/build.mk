ifeq (,$(RTEX_MODULE_VERSION))
$(error RTEX_MODULE_VERSION not defined)
endif

# TARGET_FILE sets the file stem of the target. It's typically only specified when building applications.
ifdef TARGET_FILE
TARGET_FILE_NAME = $(TARGET_FILE)
endif

# TARGET_DIR is the externally specified target directory
ifdef TARGET_DIR
TARGET_PATH = $(TARGET_DIR)
endif


ifdef APP
# when TARGET_FILE is defined on the command line,
TARGET_FILE_NAME ?= $(notdir $(APP))
TARGET_DIR_NAME ?= $(APP)
endif

ifdef APPDIR
# APPDIR is where the sources are found
# if TARGET_DIR is not defined defaults to $(APPDIR)/target
# if TARGET_FILE_NAME is not defined, defaults to the name of the $(APPDIR)
TARGET_FILE_NAME ?= $(notdir $(APPDIR))
TARGET_DIR_NAME ?= $(APPDIR)/target
# do not use $(BUILD_PATH) since the TARGET_DIR specifies fully where the output should go
ifdef TARGET_DIR
TARGET_PATH = $(TARGET_DIR)/
else
TARGET_PATH = $(TARGET_DIR_NAME)/
endif
BUILD_PATH = $(TARGET_PATH)/obj
endif

#GLOBAL_DEFINES += MODULE_VERSION=$(USER_PART_MODULE_VERSION)
#GLOBAL_DEFINES += MODULE_FUNCTION=$(MODULE_FUNCTION_USER_PART)
#GLOBAL_DEFINES += MODULE_INDEX=1
#GLOBAL_DEFINES += MODULE_DEPENDENCY=${MODULE_FUNCTION_SYSTEM_PART},2,${SYSTEM_PART2_MODULE_VERSION}

#LINKER_FILE=$(USER_PART_MODULE_PATH)/linker.ld
#LINKER_DEPS += $(LINKER_FILE)
#LINKER_DEPS += $(SYSTEM_PART2_MODULE_PATH)/module_system_part2_export.ld
#LINKER_DEPS += $(SYSTEM_PART1_MODULE_PATH)/module_system_part1_export.ld
#LINKER_DEPS += $(LIBG_TWEAK)

#LDFLAGS += -L$(SYSTEM_PART2_MODULE_PATH)
#LDFLAGS += -L$(SYSTEM_PART1_MODULE_PATH)
LDFLAGS += -L$(RTEX_MODULE_PATH)
