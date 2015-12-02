HAL_SRC_PHOBOS_PATH = src/phobos

CSRC += $(call target_files,$(HAL_SRC_PHOBOS_PATH)/,*.c)
CPPSRC += $(call target_files,$(HAL_SRC_PHOBOS_PATH)/,*.cpp)

ASRC +=

CPPFLAGS += -std=gnu++11
