/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "fallout2/fallout2.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/scummsys.h"
#include "common/system.h"
#include "engines/util.h"
#include "fallout2/console.h"
#include "fallout2/detection.h"
#include "graphics/palette.h"
#include "graphics/surface.h"

#include "fallout2/art.h"
#include "fallout2/credits.h"
#include "fallout2/critter.h"
#include "fallout2/cycle.h"
#include "fallout2/db.h"
#include "fallout2/debug.h"
#include "fallout2/draw.h"
#include "fallout2/font_manager.h"
#include "fallout2/game.h"
#include "fallout2/game_memory.h"
#include "fallout2/interface.h"
#include "fallout2/item.h"
#include "fallout2/map.h"
#include "fallout2/mainmenu.h"
#include "fallout2/memory.h"
#include "fallout2/message.h"
#include "fallout2/object.h"
#include "fallout2/palette.h"
#include "fallout2/party_member.h"
#include "fallout2/perk.h"
#include "fallout2/platform_compat.h"
#include "fallout2/proto.h"
#include "fallout2/queue.h"
#include "fallout2/random.h"
#include "fallout2/scripts.h"
#include "fallout2/settings.h"
#include "fallout2/sfall_config.h"
#include "fallout2/skill.h"
#include "fallout2/stat.h"
#include "fallout2/svga.h"
#include "fallout2/tile.h"
#include "fallout2/trait.h"
#include "fallout2/version.h"
#include "fallout2/win32.h"
#include "fallout2/window.h"
#include "fallout2/window_manager.h"

#define SPLASH_COUNT (10)

namespace Fallout2 {

Fallout2Engine *g_engine;

Fallout2Engine::Fallout2Engine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Fallout2") {
	g_engine = this;
}

Fallout2Engine::~Fallout2Engine() {
	delete _screen;
}

uint32 Fallout2Engine::getFeatures() const {
	return _gameDescription->flags;
}

Common::String Fallout2Engine::getGameId() const {
	return _gameDescription->gameId;
}

void Fallout2Engine::showSplash() {
	int splash = settings.system.splash;

	char path[64];
	const char *language = settings.system.language.c_str();
	if (compat_stricmp(language, ENGLISH) != 0) {
		snprintf(path, sizeof(path), "art\\%s\\splash\\", language);
	} else {
		snprintf(path, sizeof(path), "art\\splash\\");
	}

	File *stream;
	for (int index = 0; index < SPLASH_COUNT; index++) {
		char filePath[64];
		snprintf(filePath, sizeof(filePath), "%ssplash%d.rix", path, splash);
		debug(5, "Splash screen path: %s", filePath);
		stream = fileOpen(filePath, "rb");
		if (stream != NULL) {
			break;
		}

		splash++;

		if (splash >= SPLASH_COUNT) {
			splash = 0;
		}
	}

	if (stream == NULL) {
		return;
	}

	unsigned char *palette = reinterpret_cast<unsigned char *>(internal_malloc(768));
	if (palette == NULL) {
		fileClose(stream);
		return;
	}

	int32 version;
	if (fileReadInt32(stream, &version) == -1)
		warning("Problem reading image header");
	if (version != (int32)'RIX3') {
		fileClose(stream);
		return;
	} else
		debug(6, "Found RIX3 image signature!");

	short width;
	fileRead(&width, sizeof(width), 1, stream);

	short height;
	fileRead(&height, sizeof(height), 1, stream);

	unsigned char *data = reinterpret_cast<unsigned char *>(internal_malloc(width * height));
	if (data == NULL) {
		internal_free(palette);
		fileClose(stream);
		return;
	}

	short junk;
	paletteSetEntries(gPaletteBlack);
	//	fileSeek(stream, 10, SEEK_SET); TODO seek not implemented
	fileRead(&junk, sizeof(junk), 1, stream); // skip junk
	fileRead(palette, 1, 768, stream);
	fileRead(data, 1, width * height, stream);
	debug("Loaded splash screen!");
	fileClose(stream);

	int size = 0;

	// TODO: Move to settings.
	/*	Config config;
	if (configInit(&config)) {
		if (configRead(&config, "f2_res.ini", false)) {
			configGetInt(&config, "STATIC_SCREENS", "SPLASH_SCRN_SIZE", &size);
		}

		configFree(&config);
	}*/

	int screenWidth = screenGetWidth();
	int screenHeight = screenGetHeight();

	if (size != 0 || screenWidth < width || screenHeight < height) {
		int scaledWidth;
		int scaledHeight;

		if (size == 2) {
			scaledWidth = screenWidth;
			scaledHeight = screenHeight;
		} else {
			if (screenHeight * width >= screenWidth * height) {
				scaledWidth = screenWidth;
				scaledHeight = screenWidth * height / width;
			} else {
				scaledWidth = screenHeight * width / height;
				scaledHeight = screenHeight;
			}
		}

		unsigned char *scaled = reinterpret_cast<unsigned char *>(internal_malloc(scaledWidth * scaledHeight));
		if (scaled != NULL) {
			blitBufferToBufferStretch(data, width, height, width, scaled, scaledWidth, scaledHeight, scaledWidth);

			int x = screenWidth > scaledWidth ? (screenWidth - scaledWidth) / 2 : 0;
			int y = screenHeight > scaledHeight ? (screenHeight - scaledHeight) / 2 : 0;
			_scr_blit(scaled, scaledWidth, scaledHeight, 0, 0, scaledWidth, scaledHeight, x, y);
			paletteFadeTo(palette);

			internal_free(scaled);
		}
	} else {
		int x = (screenWidth - width) / 2;
		int y = (screenHeight - height) / 2;
		int i = 0;

		_scr_blit(data, width, height, 0, 0, width, height, x, y);
		paletteFadeTo(palette);
	}

	internal_free(data);
	internal_free(palette);

	settings.system.splash = splash + 1;
}


/*
int isoInit() {
	tileScrollLimitingDisable();
	tileScrollBlockingDisable();

	// NOTE: Uninline.
	//square_init();

	if (artInit() != 0) {
		debugPrint("art_init failed in iso_init\n");
		return -1;
	}

	debugPrint(">art_init\t\t");

//	if (tileInit(_square, SQUARE_GRID_WIDTH, SQUARE_GRID_HEIGHT, HEX_GRID_WIDTH, HEX_GRID_HEIGHT, gIsoWindowBuffer, screenGetWidth(), screenGetVisibleHeight(), screenGetWidth(), isoWindowRefreshRect) != 0) {
//		debugPrint("tile_init failed in iso_init\n");
//		return -1;
//	}

//	debugPrint(">tile_init\t\t");

	// if (objectsInit(gIsoWindowBuffer, screenGetWidth(), screenGetVisibleHeight(), screenGetWidth()) != 0) {
	// 	debugPrint("obj_init failed in iso_init\n");
	// 	return -1;
	// }

	// debugPrint(">obj_init\t\t");

	// colorCycleInit();
	// debugPrint(">cycle_init\t\t");

	// tileScrollBlockingEnable();
	// tileScrollLimitingEnable();

	if (interfaceInit() != 0) {
		debugPrint("intface_init failed in iso_init\n");
		return -1;
	}

	debugPrint(">intface_init\t\t");

	// SFALL
//	mapMakeMapsDirectory();

	// NOTE: Uninline.
//	mapSetEnteringLocation(-1, -1, -1);

	return 0;
}*/

Common::Error Fallout2Engine::run() {
	// Initialize 640x480 paletted graphics mode
	initGraphics(640, 480);
	_screen = new Graphics::Screen();

	// Set the engine's debugger console
	setDebugger(new Console());

	// If a savegame was selected from the launcher, load it
	int saveSlot = ConfMan.getInt("save_slot");
	if (saveSlot != -1)
		(void)loadGameState(saveSlot);

	// Draw a series of boxes on screen as a sample
	//	for (int i = 0; i < 100; ++i)
	//		_screen->frameRect(Common::Rect(i, i, 320 - i, 200 - i), i);
	//	_screen->update();

	// Set empty palette
	byte pal[256 * 3] = { 0 };
	g_system->getPaletteManager()->setPalette(pal, 0, 256);

	int offset = 0;

	Common::Event e;

	// Print engine version
	char version[100];
	versionGetVersion(version, sizeof(version));
	debug("%s", version);

	gProgramIsActive = true;

	// Init memory manager
	if (gameMemoryInit() == -1)
		warning("Error allocating memory");
	else
		debug("Memory allocation successful!");

	// Sfall config should be initialized before game config, since it can
	// override it's file name.
	sfallConfigInit(1, nullptr);

	// Init game settings
	if (settingsInit(false, 1, nullptr))
		debug("Settings initialized!");
	else
		warning("Couldn't init settings");

	// Init resources
	if (gameDbInit() != -1)
		debug("Databases opened!");
	else
		warning("Couldn't open databases");

	// Message list repository is considered a specialized file manager, so
	// it should be initialized early in the process.
	// Initialize messages repository
	if (messageListRepositoryInit() == true)
		debug("Initialized message repository!");
	else
		warning("Couldn't initialize message repository");

	// init main window
	_initWindow(1, 0);
	debug("Initialized window manager!");

	// init game palette
	paletteInit();

	// Init game-specific RNG
	randomInit();
	debug("Initialized RNG!");

	// show splash screen
	settings.system.splash = getRandomNumber(6); // 6 possible splashscreens
	showSplash();
	_screen->update();

	// init fonts
	interfaceFontsInit();
	fontManagerAdd(&gModernFontManager);
	fontSetCurrent(0);  // this does nothing, font IDs are >100
	debug("Initialized interface fonts!");

	tileDisable();

	badwordsInit();

	// init skills messagelist
	if (skillsInit() == 0)
		debug("Initialized skills!");
	else
		warning("Error initializing skills");

	// init stats messagelist
	if (statsInit() == 0)
		debug("Initialized stats!");
	else
		warning("Error initializing stats");

	if (partyMembersInit() != 0)
		warning("Failed on partyMember_init");
	else
		debug("Initialized party members!");

	if (perksInit() == 0)
		debug("Initialized perks!");
	else
		warning("Error initializing perks");

	if (traitsInit() == true)
		debug("Initialized traits!");
	else
		warning("Error initializing traits");

	if (itemsInit() == 0)
		debug("Initialized items!");
	else
		warning("Error initializing items!");

	queueInit();

	if (critterInit() == 0)
		debug("Initialized critters!");
	else
		warning("Error initializing critters");

	if (isoInit() != 0)
		warning("Failed on iso_init");

	if (protoInit() != 0)
		warning("Failed on proto_init");
	else
		debug("Initialized protos!");

	if (scriptsInit() != 0)
		warning("Failed on scripts init");
	else
		debug("Initialized scripts!");

	char path[COMPAT_MAX_PATH];

	if (!messageListInit(&gMiscMessageList))
		warning("Failed on message_init\n");
	else
		debug("Initialized messages list!");

	snprintf(path, sizeof(path), "%s%s", asc_5186C8, "misc.msg");

	if (!messageListLoad(&gMiscMessageList, path))
		warning("Failed on message_load\n");
	else
		debug("Loaded messages list: %s!", path);

	messageListRepositorySetStandardMessageList(STANDARD_MESSAGE_LIST_MISC, &gMiscMessageList);

	if (mainMenuWindowInit() == 0)
		debug("Initialized main menu!");
	else
		warning("Couldn't load main menu");

	// throw a dice (yay!)
	debugPrint("RandomRoll (diff= 70) result: %d", randomRoll(70, 5, NULL));
	debugPrint("RandomRoll (diff= 10) result: %d", randomRoll(10, 5, NULL));
	debugPrint("RandomRoll (diff= 150) result: %d", randomRoll(150, 5, NULL));

	g_system->delayMillis(1000);
	paletteFadeTo(gPaletteBlack);

	bool exit = false;
	mainMenuWindowUnhide(1);
	while (!shouldQuit() && !exit) {
		while (g_system->getEventManager()->pollEvent(e)) {

			if (e.type == Common::EVENT_SCREEN_CHANGED)
				_screen->update();
			if (e.type == Common::EVENT_LBUTTONDOWN) {
				exit = true;
				break;
			}

			// Delay for a bit. All events loops should have a delay
			// to prevent the system being unduly loaded
			g_system->delayMillis(10);
		}
	}
	mainMenuWindowHide(1);
	creditsOpen("credits.txt", -1, false);
	gameExit();
	return Common::kNoError;
}

Common::Error Fallout2Engine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

} // End of namespace Fallout2
