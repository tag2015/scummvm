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
#include "fallout2/detection.h"
#include "fallout2/console.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/palette.h"

#include "fallout2/version.h"
#include "fallout2/win32.h"
#include "fallout2/random.h"
#include "fallout2/game_memory.h"
#include "fallout2/settings.h"
#include "fallout2/debug.h"
#include "fallout2/game.h"
#include "fallout2/message.h"
#include "fallout2/window.h"


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

Common::Error Fallout2Engine::run() {
	// Initialize 320x200 paletted graphics mode
	initGraphics(320, 200);
	_screen = new Graphics::Screen();

	// Set the engine's debugger console
	setDebugger(new Console());

	// If a savegame was selected from the launcher, load it
	int saveSlot = ConfMan.getInt("save_slot");
	if (saveSlot != -1)
		(void)loadGameState(saveSlot);

	// Draw a series of boxes on screen as a sample
	for (int i = 0; i < 100; ++i)
		_screen->frameRect(Common::Rect(i, i, 320 - i, 200 - i), i);
	_screen->update();

	// Simple event handling loop
	byte pal[256 * 3] = { 0 };
	Common::Event e;
	int offset = 0;

	char version[100];
	versionGetVersion(version, 50);
	debug("%s",version);
	gProgramIsActive = true;
	randomInit(); // specific engine rng init
	if (gameMemoryInit() == -1)
		warning("Error allocating memory");
	else
		debug("Fallout2: memory allocation successful!");

	char test_argv[10][512];
	strncpy(test_argv[0],"fallout2.exe",sizeof(test_argv[0]));
	char *tmp_argv = test_argv[0];
	char **tmp_argv2 = &tmp_argv;

	if(settingsInit(false, 1, tmp_argv2))
		debug("Settings initialized!");
	else
		warning("can't init settings");

	if (gameDbInit() != -1)
		debug("Databases opened");
	else
		warning("can't open databases!");

	// Message list repository is considered a specialized file manager, so
	// it should be initialized early in the process.
	if(messageListRepositoryInit() == true)
		debug("Initialized message repository");
	else
		warning("Couldn't initialize message repo");

//	_initWindow(1,0);
	// throw a dice (yay!)
	debugPrint("RandomRoll (diff= 70) result: %d", randomRoll(70, 5, NULL));
	debugPrint("RandomRoll (diff= 10) result: %d", randomRoll(10, 5, NULL));
	debugPrint("RandomRoll (diff= 150) result: %d", randomRoll(150, 5, NULL));
	while (!shouldQuit()) {
		while (g_system->getEventManager()->pollEvent(e)) {
		}

		// Cycle through a simple palette
		++offset;
		for (int i = 0; i < 256; ++i)
			pal[i * 3 + 1] = (i + offset) % 256;
		g_system->getPaletteManager()->setPalette(pal, 0, 256);
		_screen->update();

		// Delay for a bit. All events loops should have a delay
		// to prevent the system being unduly loaded
		g_system->delayMillis(10);
	}

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
