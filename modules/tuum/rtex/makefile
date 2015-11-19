MODULE=rtex

PROJECT_ROOT=../../..
RTEX_MODULE_PATH=.

BUILD_PATH_EXT = $(BUILD_TARGET_PLATFORM)
HAL_LINK :=

DEPENDENCIES = user hal system vision loc entities geometry rtxmath
MAKE_DEPENDENCIES = user hal system vision loc entities geometry rtxmath

include ../modular.mk
include $(PROJECT_ROOT)/build/platform-id.mk

LIBS += $(MAKE_DEPENDENCIES) pthread boost_system

LIBS += gtkmm-3.0 atkmm-1.6 gdkmm-3.0 giomm-2.4 pangomm-1.4 glibmm-2.4 gtk-3 gdk-3 pangocairo-1.0 pango-1.0 atk-1.0 cairo-gobject gio-2.0 cairomm-1.0 cairo sigc-2.0 gdk_pixbuf-2.0 gobject-2.0 glib-2.0 # FIXME: Libraries added due to gtkmm usage; should be LDLIBS += `pkg-config gtkmm-3.0 --libs`

LIB_DEPS += $(USER_LIB_DEP) $(VISION_LIB_DEP) $(SYSTEM_LIB_DEP) $(HAL_LIB_DEP) $(LOC_LIB_DEP) $(ENTITIES_LIB_DEP) $(GEOMETRY_LIB_DEP) $(RTXMATH_LIB_DEP)

LIB_DIRS += $(dir $(LIB_DEPS))

LDFLAGS +=

TARGET=out elf


include $(PROJECT_ROOT)/build/tlm.mk

#$(call check_modular)
