MODULE=user
USER_MODULE_PATH=.

BUILD_PATH_EXT = $(USER_BUILD_PATH_EXT)

#FIXME: Perhaps separate system module could be made dependant on platform instead of user module?
DEPENDENCIES = hal vision geometry entities platform loc system

LIB_DEPS +=

include ../build/platform-id.mk

WARNINGS_AS_ERRORS ?= n

TARGET_TYPE = a

include ../build/tlm.mk
