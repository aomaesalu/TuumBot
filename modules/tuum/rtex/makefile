MODULE=rtex

PROJECT_ROOT=../../..
RTEX_MODULE_PATH=.

BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)
HAL_LINK :=

DEPENDENCIES = user hal system vision loc entities geometry rtxmath
MAKE_DEPENDENCIES = user hal system vision loc entities geometry rtxmath

include ../modular.mk
include $(PROJECT_ROOT)/build/platform-id.mk

LIBS += $(MAKE_DEPENDENCIES) boost_system

LIB_DEPS += $(USER_LIB_DEP) $(VISION_LIB_DEP) $(SYSTEM_LIB_DEP) $(HAL_LIB_DEP) $(LOC_LIB_DEP) $(ENTITIES_LIB_DEP) $(GEOMETRY_LIB_DEP) $(RTXMATH_LIB_DEP)

LIB_DIRS += $(dir $(LIB_DEPS))

LDFLAGS +=

TARGET=out elf


include $(PROJECT_ROOT)/build/tlm.mk

#$(call check_modular)
