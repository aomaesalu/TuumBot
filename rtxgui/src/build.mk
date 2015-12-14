# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# C source files included in this build.
CSRC +=

# enumerate target cpp files
CPPSRC += $(call target_files,src/,*.cpp)

# ASM source files included in this build.
ASRC +=

CPPFLAGS += -std=gnu++11 -fPIC

BUILTINS_EXCLUDE =
CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

INCLUDE_DIRS += /usr/include/x86_64-linux-gnu/qt5 /usr/include/x86_64-linux-gnu/qt5/QtWidgets /usr/include/x86_64-linux-gnu/qt5/QtGui /usr/include/x86_64-linux-gnu/qt5/QtCore /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64
