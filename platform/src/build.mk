
TARGET_SRC_PATH = src

CSRC +=
CPPSRC += $(call target_files,src/,*.cpp)

ASRC +=

CPPFLAGS += -std=gnu++11
