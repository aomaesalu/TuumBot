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

<<<<<<< fd7077e5b6495469d2d04fab5397af789a109ce1
CPPFLAGS += -std=gnu++11
=======
CPPFLAGS += -std=gnu++14
>>>>>>> Create build module for system modules.
