# (C) 2002-2006 James R. Bruce, Carnegie Mellon University
# Licenced under the GNU General Public License (GPL) version 2,
#   or alternately by a specific written agreement.

# set bin prefix
BIN := bin

CPUFLAGS_BIN := ./bin/cpuflags
FIXDEPEND_BIN := ./bin/fixdepend

CPU := $(shell $(CPUFLAGS_BIN))

#======================================================================#
# Compilation options
#======================================================================#

# CC := distcc g++
CC := g++
CFLAGS := -Wall -D_GNU_SOURCE
CFLAGS += $(CPU)
CFLAGS += -ffast-math

# CFLAGS += -O2 -g -pg
# CFLAGS += -O0 -g
# CFLAGS += -O1 -g
CFLAGS += -O2 -g
# CFLAGS += -O2
# CFLAGS += -O2

CFLAGS += -std=c++11 -march=x86-64

# uncomment to turn off asserts
# CFLAGS += -DNDEBUG

#======================================================================#

DEP_ECHO  = @echo Dependency: $@
CC_ECHO   = @echo Compiling: $@
LINK_ECHO = @echo Linking: $@
CLEAN_ECHO = @echo Cleaning.

ifneq ($(VERBOSE), 1)
  Q := @
endif

#======================================================================#
# Build Setup
#======================================================================#

# automatic include path
CFLAGS += -I. -Ishared -Icmvision

# libraries
LIBS := -lz
LFLAGS :=

# Each target will add to this
ALL_SRC :=
ALL_TARGETS :=

all: all_real

#======================================================================#
# Common Sources
#======================================================================#

COMMON_SRC := $(filter-out %_test.cc, $(wildcard shared/*.cc))
VISION_SRC := $(filter-out %_test.cc, $(wildcard cmvision/*.cc))

#======================================================================#
# Example Program
#======================================================================#

SRC := test/hello.cc
SRC += $(COMMON_SRC) $(VISION_SRC)
TARGET := cmv_hello

HELLO_OBJ := $(SRC:.cc=.o)

ALL_SRC += $(SRC)
ALL_TARGETS += $(TARGET)

$(TARGET): $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(HELLO_OBJ)
	$(LINK_ECHO)
	$(Q)$(CC) -o $@ $(CFLAGS) $(LFLAGS) $(HELLO_OBJ) $(LIBS)

#======================================================================#
# Build Rules
#======================================================================#

# Determine the object files
OBJ := \
	$(patsubst %.cc,%.o,$(filter %.cc,$(ALL_SRC))) \
	$(patsubst %.c,%.o,$(filter %.c,$(ALL_SRC)))

# Determine the target files
ALL_BIN_TARGETS := $(addprefix $(BIN)/,$(ALL_TARGETS))

# Include the dependencies
-include $(OBJ:.o=.d)

# ignore implicit rules (I think)
.SUFFIXES:

# rules to run unconditionally
# (i.e. those which do not generate a file the same name as the target)
.PHONY: sloc strip doc clean cleanbin cleandoc distclean

# Compilation rule
%.o: %.cc
	$(CC_ECHO)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

# Dependency generation rule
%.d: %.cc
	$(DEP_ECHO)
	$(Q)$(CC) $(CFLAGS) -M $< | $(FIXDEPEND_BIN) $(dir $<) > $@

all_real: $(ALL_BIN_TARGETS)

strip:
	strip -v --strip-unneeded $(ALL_BIN_TARGETS)

clean:
	$(CLEAN_ECHO)
	$(Q)find . -type f -name "*.[od]" | xargs rm -f
	$(Q)find . -type f -name "*~" | xargs rm -f

cleanbin:
	rm -f $(ALL_BIN_TARGETS)

cleandoc:
	(cd doc/html; rm -f doxygen.css *.html *.png *.map *.md5 *.dot)

distclean: clean cleanbin cleandoc

# don't need to do anything extra since dependencies are built automatically
depend:
