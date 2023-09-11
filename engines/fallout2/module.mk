MODULE := engines/fallout2

MODULE_OBJS = \
	fallout2.o \
	console.o \
	metaengine.o \
	art.o \
	cache.o \
	color.o \
	config.o \
	critter.o \
	cycle.o \
	datafile.o \
	db.o \
	dfile.o \
	dictionary.o \
	dinput.o \
	draw.o \
	export.o \
	file_find.o \
	font_manager.o \
	game.o \
	game_config.o \
	game_memory.o \
	geometry.o \
	graph_lib.o \
	heap.o \
	input.o \
	interpreter.o \
	light.o \
	memory.o \
	memory_manager.o \
	message.o \
	object.o \
	palette.o \
	party_member.o \
	pcx.o \
	perk.o \
	platform_compat.o \
	pointer_registry.o \
	proto.o \
	queue.o \
	random.o \
	reaction.o \
	region.o \
	settings.o \
	sfall_config.o \
	skill.o \
	stat.o \
	string_parsers.o \
	text_font.o \
	text_object.o \
	tile.o \
	trait.o \
	version.o \
	widget.o \
	window.o \
	word_wrap.o \
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
