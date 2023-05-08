MODULE := engines/fallout2

MODULE_OBJS = \
	fallout2.o \
	console.o \
	metaengine.o \
	cache.o \
	heap.o \
	memory.o \
	memory_manager.o \
	platform_compat.o \
	version.o

# This module can be built as a plugin
ifeq ($(ENABLE_FALLOUT2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
