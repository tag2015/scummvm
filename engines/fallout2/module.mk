MODULE := engines/fallout2

MODULE_OBJS = \
	fallout2.o \
	console.o \
	metaengine.o \
	cache.o \
	color.o \
	config.o \
	cycle.o \
	db.o \
	dfile.o \
	dictionary.o \
	draw.o \
	file_find.o \
	font_manager.o \
	game_config.o \
	game_memory.o \
	geometry.o \
	graph_lib.o \
	heap.o \
	memory.o \
	memory_manager.o \
	palette.o \
	pcx.o \
	platform_compat.o \
	random.o \
	settings.o \
	text_font.o \
	version.o \
	xfile.o \
	third_party/fpattern/fpattern.o

# This module can be built as a plugin
ifeq ($(ENABLE_FALLOUT2), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
