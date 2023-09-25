MODULE := engines/fallout2

MODULE_OBJS = \
	fallout2.o \
	console.o \
	metaengine.o \
	actions.o \
	art.o \
	animation.o \
	cache.o \
	character_editor.o \
	character_selector.o \
	color.o \
	combat.o \
	combat_ai.o \
	config.o \
	credits.o \
	critter.o \
	cycle.o \
	datafile.o \
	db.o \
	dbox.o \
	dfile.o \
	dialog.o \
	dictionary.o \
	dinput.o \
	display_monitor.o \
	draw.o \
	elevator.o \
	export.o \
	file_find.o \
	font_manager.o \
	fps_limiter.o \
	game.o \
	game_dialog.o \
	game_config.o \
	game_memory.o \
	game_mouse.o \
	geometry.o \
	graph_lib.o \
	heap.o \
	input.o \
	interface.o \
	interpreter.o \
	interpreter_extra.o \
	interpreter_lib.o \
	inventory.o \
	item.o \
	light.o \
	lips.o \
	mainmenu.o \
	memory.o \
	memory_manager.o \
	map.o \
	message.o \
	mouse.o \
	mouse_manager.o \
	nevs.o \
	object.o \
	options.o \
	palette.o \
	party_member.o \
	pcx.o \
	perk.o \
	platform_compat.o \
	pointer_registry.o \
	preferences.o \
	proto.o \
	proto_instance.o \
	queue.o \
	random.o \
	reaction.o \
	region.o \
	scripts.o \
	select_file_list.o \
	settings.o \
	sfall_config.o \
	skill.o \
	skilldex.o \
	stat.o \
	string_parsers.o \
	svga.o \
	text_font.o \
	text_object.o \
	tile.o \
	trait.o \
	version.o \
	widget.o \
	win32.o \
	window.o \
	window_manager.o \
	window_manager_private.o \
	word_wrap.o \
	worldmap.o \
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
