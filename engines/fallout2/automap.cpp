#include "fallout2/automap.h"

/*#include <stdio.h>
#include <string.h>

#include <algorithm>*/

#include "fallout2/fallout2.h"
#include "fallout2/art.h"
#include "fallout2/color.h"
#include "fallout2/config.h"
#include "fallout2/dbox.h"
#include "fallout2/debug.h"
#include "fallout2/draw.h"
#include "fallout2/game.h"
#include "fallout2/game_mouse.h"
#include "fallout2/game_sound.h"
#include "fallout2/graph_lib.h"
#include "fallout2/input.h"
#include "fallout2/item.h"
#include "fallout2/kb.h"
#include "fallout2/map.h"
#include "fallout2/memory.h"
#include "fallout2/object.h"
#include "fallout2/platform_compat.h"
#include "fallout2/settings.h"
#include "fallout2/svga.h"
#include "fallout2/text_font.h"
#include "fallout2/window_manager.h"

#include "common/config-manager.h"
#include "common/stream.h"


namespace Fallout2 {

#define AUTOMAP_OFFSET_COUNT (AUTOMAP_MAP_COUNT * ELEVATION_COUNT)

#define AUTOMAP_WINDOW_WIDTH (519)
#define AUTOMAP_WINDOW_HEIGHT (480)

#define AUTOMAP_PIPBOY_VIEW_X (238)
#define AUTOMAP_PIPBOY_VIEW_Y (105)

static void automapRenderInMapWindow(int window, int elevation, unsigned char *backgroundData, int flags);
static int automapSaveEntry(Common::WriteStream *stream);
static int automapLoadEntry(int map, int elevation);
static int automapSaveHeader(Common::SeekableWriteStream *stream);
static int automapLoadHeader(Common::ReadStream *stream);
static void _decode_map_data(int elevation);
static int automapCreate();
static int _copy_file_data(Common::ReadStream *stream1, Common::WriteStream *stream2, int length);

typedef enum AutomapFrm {
	AUTOMAP_FRM_BACKGROUND,
	AUTOMAP_FRM_BUTTON_UP,
	AUTOMAP_FRM_BUTTON_DOWN,
	AUTOMAP_FRM_SWITCH_UP,
	AUTOMAP_FRM_SWITCH_DOWN,
	AUTOMAP_FRM_COUNT,
} AutomapFrm;

typedef struct AutomapEntry {
	int dataSize;
	unsigned char isCompressed;
	unsigned char *compressedData;
	unsigned char *data;
} AutomapEntry;

// 0x41ADE0
static const int _defam[AUTOMAP_MAP_COUNT][ELEVATION_COUNT] = {
	{-1, -1, -1},
	{-1, -1, -1},
	{-1, -1, -1},
};

// 0x41B560
static int _displayMapList[AUTOMAP_MAP_COUNT] = {
	-1,
	-1,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-1,
	0,
	0,
	0,
	0,
	0,
	-1,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	0,
	0,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
};

// 0x41B7E0
static const int gAutomapFrmIds[AUTOMAP_FRM_COUNT] = {
	171, // automap.frm - automap window
	8,   // lilredup.frm - little red button up
	9,   // lilreddn.frm - little red button down
	172, // autoup.frm - switch up
	173, // autodwn.frm - switch down
};

// 0x5108C4
static int gAutomapFlags = 0;

// 0x56CB18
static AutomapHeader gAutomapHeader;

// 0x56D2A0
static AutomapEntry gAutomapEntry;

// automap_init
// 0x41B7F4
int automapInit() {
	gAutomapFlags = 0;
	automapCreate();
	return 0;
}

// 0x41B808
int automapReset() {
	gAutomapFlags = 0;
	automapCreate();
	return 0;
}

// 0x41B81C
void automapExit() {
	Common::SaveFileManager *saveMan = g_system->getSavefileManager();
	Common::Path automapPath(g_engine->getTargetName());

	automapPath.appendInPlace("_");
	automapPath.appendInPlace(AUTOMAP_DB);
	if (saveMan->exists(automapPath.toConfig()))
		saveMan->removeSavefile(automapPath.toConfig());

	//	char path[COMPAT_MAX_PATH];
	//	snprintf(path, sizeof(path), "%s\\%s\\%s", settings.system.master_patches_path.c_str(), "MAPS", AUTOMAP_DB);
	//	compat_remove(path);
}

// 0x41B87C
int automapLoad(Common::InSaveFile *stream) {
	gAutomapFlags = stream->readSint32BE();
	if (stream->err())
		return -1;
	return 0;
}

// 0x41B898
int automapSave(Common::OutSaveFile *stream) {
	stream->writeSint32BE(gAutomapFlags);
	if (stream->err()) {
		stream->finalize();
		delete stream;
		return -1;
	}
	return 0;
}

// 0x41B8B4
int _automapDisplayMap(int map) {
	return _displayMapList[map];
}

// 0x41B8BC
void automapShow(bool isInGame, bool isUsingScanner) {
	ScopedGameMode gm(GameMode::kAutomap);

	int frmIds[AUTOMAP_FRM_COUNT];
	memcpy(frmIds, gAutomapFrmIds, sizeof(gAutomapFrmIds));

	FrmImage frmImages[AUTOMAP_FRM_COUNT];
	for (int index = 0; index < AUTOMAP_FRM_COUNT; index++) {
		int fid = buildFid(OBJ_TYPE_INTERFACE, frmIds[index], 0, 0, 0);
		if (!frmImages[index].lock(fid)) {
			return;
		}
	}

	int color;
	if (isInGame) {
		color = _colorTable[8456];
		_obj_process_seen();
	} else {
		color = _colorTable[22025];
	}

	int oldFont = fontGetCurrent();
	fontSetCurrent(101);

	int automapWindowX = (screenGetWidth() - AUTOMAP_WINDOW_WIDTH) / 2;
	int automapWindowY = (screenGetHeight() - AUTOMAP_WINDOW_HEIGHT) / 2;
	int window = windowCreate(automapWindowX, automapWindowY, AUTOMAP_WINDOW_WIDTH, AUTOMAP_WINDOW_HEIGHT, color, WINDOW_MODAL | WINDOW_MOVE_ON_TOP);

	int scannerBtn = buttonCreate(window,
								  111,
								  454,
								  15,
								  16,
								  -1,
								  -1,
								  -1,
								  KEY_LOWERCASE_S,
								  frmImages[AUTOMAP_FRM_BUTTON_UP].getData(),
								  frmImages[AUTOMAP_FRM_BUTTON_DOWN].getData(),
								  nullptr,
								  BUTTON_FLAG_TRANSPARENT);
	if (scannerBtn != -1) {
		buttonSetCallbacks(scannerBtn, _gsound_red_butt_press, _gsound_red_butt_release);
	}

	int cancelBtn = buttonCreate(window,
								 277,
								 454,
								 15,
								 16,
								 -1,
								 -1,
								 -1,
								 KEY_ESCAPE,
								 frmImages[AUTOMAP_FRM_BUTTON_UP].getData(),
								 frmImages[AUTOMAP_FRM_BUTTON_DOWN].getData(),
								 nullptr,
								 BUTTON_FLAG_TRANSPARENT);
	if (cancelBtn != -1) {
		buttonSetCallbacks(cancelBtn, _gsound_red_butt_press, _gsound_red_butt_release);
	}

	int switchBtn = buttonCreate(window,
								 457,
								 340,
								 42,
								 74,
								 -1,
								 -1,
								 KEY_LOWERCASE_L,
								 KEY_LOWERCASE_H,
								 frmImages[AUTOMAP_FRM_SWITCH_UP].getData(),
								 frmImages[AUTOMAP_FRM_SWITCH_DOWN].getData(),
								 nullptr,
								 BUTTON_FLAG_TRANSPARENT | BUTTON_FLAG_0x01);
	if (switchBtn != -1) {
		buttonSetCallbacks(switchBtn, _gsound_toggle_butt_press_, _gsound_toggle_butt_press_);
	}

	if ((gAutomapFlags & AUTOMAP_WTH_HIGH_DETAILS) == 0) {
		_win_set_button_rest_state(switchBtn, 1, 0);
	}

	int elevation = gElevation;

	gAutomapFlags &= AUTOMAP_WTH_HIGH_DETAILS;

	if (isInGame) {
		gAutomapFlags |= AUTOMAP_IN_GAME;
	}

	if (isUsingScanner) {
		gAutomapFlags |= AUTOMAP_WITH_SCANNER;
	}

	automapRenderInMapWindow(window, elevation, frmImages[AUTOMAP_FRM_BACKGROUND].getData(), gAutomapFlags);

	bool isoWasEnabled = isoDisable();
	gameMouseSetCursor(MOUSE_CURSOR_ARROW);

	bool done = false;
	while (!done) {
		sharedFpsLimiter.mark();

		bool needsRefresh = false;

		// FIXME: There is minor bug in the interface - pressing H/L to toggle
		// high/low details does not update switch state.
		int keyCode = inputGetInput();
		switch (keyCode) {
		case KEY_TAB:
		case KEY_ESCAPE:
		case KEY_UPPERCASE_A:
		case KEY_LOWERCASE_A:
			done = true;
			break;
		case KEY_UPPERCASE_H:
		case KEY_LOWERCASE_H:
			if ((gAutomapFlags & AUTOMAP_WTH_HIGH_DETAILS) == 0) {
				gAutomapFlags |= AUTOMAP_WTH_HIGH_DETAILS;
				needsRefresh = true;
			}
			break;
		case KEY_UPPERCASE_L:
		case KEY_LOWERCASE_L:
			if ((gAutomapFlags & AUTOMAP_WTH_HIGH_DETAILS) != 0) {
				gAutomapFlags &= ~AUTOMAP_WTH_HIGH_DETAILS;
				needsRefresh = true;
			}
			break;
		case KEY_UPPERCASE_S:
		case KEY_LOWERCASE_S:
			if (elevation != gElevation) {
				elevation = gElevation;
				needsRefresh = true;
			}

			if ((gAutomapFlags & AUTOMAP_WITH_SCANNER) == 0) {
				Object *scanner = nullptr;

				Object *item1 = critterGetItem1(gDude);
				if (item1 != nullptr && item1->pid == PROTO_ID_MOTION_SENSOR) {
					scanner = item1;
				} else {
					Object *item2 = critterGetItem2(gDude);
					if (item2 != nullptr && item2->pid == PROTO_ID_MOTION_SENSOR) {
						scanner = item2;
					}
				}

				if (scanner != nullptr && miscItemGetCharges(scanner) > 0) {
					needsRefresh = true;
					gAutomapFlags |= AUTOMAP_WITH_SCANNER;
					miscItemConsumeCharge(scanner);
				} else {
					soundPlayFile("iisxxxx1");

					MessageListItem messageListItem;
					// 17 - The motion sensor is not installed.
					// 18 - The motion sensor has no charges remaining.
					const char *title = getmsg(&gMiscMessageList, &messageListItem, scanner != nullptr ? 18 : 17);
					showDialogBox(title, nullptr, 0, 165, 140, _colorTable[32328], nullptr, _colorTable[32328], 0);
				}
			}

			break;
		case KEY_CTRL_Q:
		case KEY_ALT_X:
		case KEY_F10:
			showQuitConfirmationDialog();
			break;
		case KEY_F12:
			takeScreenshot();
			break;
		}

		if (_game_user_wants_to_quit != 0) {
			break;
		}

		if (needsRefresh) {
			automapRenderInMapWindow(window, elevation, frmImages[AUTOMAP_FRM_BACKGROUND].getData(), gAutomapFlags);
			needsRefresh = false;
		}

		renderPresent();
		sharedFpsLimiter.throttle();
	}

	if (isoWasEnabled) {
		isoEnable();
	}

	windowDestroy(window);
	fontSetCurrent(oldFont);
}

// Renders automap in Map window.
//
// 0x41BD1C
static void automapRenderInMapWindow(int window, int elevation, unsigned char *backgroundData, int flags) {
	int color;
	if ((flags & AUTOMAP_IN_GAME) != 0) {
		color = _colorTable[8456];
	} else {
		color = _colorTable[22025];
	}

	windowFill(window, 0, 0, AUTOMAP_WINDOW_WIDTH, AUTOMAP_WINDOW_HEIGHT, color);
	windowDrawBorder(window);

	unsigned char *windowBuffer = windowGetBuffer(window);
	blitBufferToBuffer(backgroundData, AUTOMAP_WINDOW_WIDTH, AUTOMAP_WINDOW_HEIGHT, AUTOMAP_WINDOW_WIDTH, windowBuffer, AUTOMAP_WINDOW_WIDTH);

	for (Object *object = objectFindFirstAtElevation(elevation); object != nullptr; object = objectFindNextAtElevation()) {
		if (object->tile == -1) {
			continue;
		}

		int objectType = FID_TYPE(object->fid);
		unsigned char objectColor;

		if ((flags & AUTOMAP_IN_GAME) != 0) {
			if (objectType == OBJ_TYPE_CRITTER && (object->flags & OBJECT_HIDDEN) == 0 && (flags & AUTOMAP_WITH_SCANNER) != 0 && (object->data.critter.combat.results & DAM_DEAD) == 0) {
				objectColor = _colorTable[31744];
			} else {
				if ((object->flags & OBJECT_SEEN) == 0) {
					continue;
				}

				if (object->pid == PROTO_ID_0x2000031) {
					objectColor = _colorTable[32328];
				} else if (objectType == OBJ_TYPE_WALL) {
					objectColor = _colorTable[992];
				} else if (objectType == OBJ_TYPE_SCENERY && (flags & AUTOMAP_WTH_HIGH_DETAILS) != 0 && object->pid != PROTO_ID_0x2000158) {
					objectColor = _colorTable[480];
				} else if (object == gDude) {
					objectColor = _colorTable[31744];
				} else {
					objectColor = _colorTable[0];
				}
			}
		}

		int v10 = -2 * (object->tile % 200) - 10 + AUTOMAP_WINDOW_WIDTH * (2 * (object->tile / 200) + 9) - 60;
		if ((flags & AUTOMAP_IN_GAME) == 0) {
			switch (objectType) {
			case OBJ_TYPE_ITEM:
				objectColor = _colorTable[6513];
				break;
			case OBJ_TYPE_CRITTER:
				objectColor = _colorTable[28672];
				break;
			case OBJ_TYPE_SCENERY:
				objectColor = _colorTable[448];
				break;
			case OBJ_TYPE_WALL:
				objectColor = _colorTable[12546];
				break;
			case OBJ_TYPE_MISC:
				objectColor = _colorTable[31650];
				break;
			default:
				objectColor = _colorTable[0];
			}
		}

		if (objectColor != _colorTable[0]) {
			unsigned char *v12 = windowBuffer + v10;
			if ((flags & AUTOMAP_IN_GAME) != 0) {
				if (*v12 != _colorTable[992] || objectColor != _colorTable[480]) {
					v12[0] = objectColor;
					v12[1] = objectColor;
				}

				if (object == gDude) {
					v12[-1] = objectColor;
					v12[-AUTOMAP_WINDOW_WIDTH] = objectColor;
					v12[AUTOMAP_WINDOW_WIDTH] = objectColor;
				}
			} else {
				v12[0] = objectColor;
				v12[1] = objectColor;
				v12[AUTOMAP_WINDOW_WIDTH] = objectColor;
				v12[AUTOMAP_WINDOW_WIDTH + 1] = objectColor;

				v12[AUTOMAP_WINDOW_WIDTH - 1] = objectColor;
				v12[AUTOMAP_WINDOW_WIDTH + 2] = objectColor;
				v12[AUTOMAP_WINDOW_WIDTH * 2] = objectColor;
				v12[AUTOMAP_WINDOW_WIDTH * 2 + 1] = objectColor;
			}
		}
	}

	int textColor;
	if ((flags & AUTOMAP_IN_GAME) != 0) {
		textColor = _colorTable[992];
	} else {
		textColor = _colorTable[12546];
	}

	if (mapGetCurrentMap() != -1) {
		char *areaName = mapGetCityName(mapGetCurrentMap());
		windowDrawText(window, areaName, 240, 150, 380, textColor | 0x2000000);

		char *mapName = mapGetName(mapGetCurrentMap(), elevation);
		windowDrawText(window, mapName, 240, 150, 396, textColor | 0x2000000);
	}

	windowRefresh(window);
}

// Renders automap in Pipboy window.
//
// 0x41C004
int automapRenderInPipboyWindow(int window, int map, int elevation) {
	unsigned char *windowBuffer = windowGetBuffer(window) + 640 * AUTOMAP_PIPBOY_VIEW_Y + AUTOMAP_PIPBOY_VIEW_X;

	unsigned char wallColor = _colorTable[992];
	unsigned char sceneryColor = _colorTable[480];

	gAutomapEntry.data = (unsigned char *)internal_malloc(11024);
	if (gAutomapEntry.data == nullptr) {
		debugPrint("\nAUTOMAP: Error allocating data buffer!\n");
		return -1;
	}

	if (automapLoadEntry(map, elevation) == -1) {
		internal_free(gAutomapEntry.data);
		return -1;
	}

	int v1 = 0;
	unsigned char v2 = 0;
	unsigned char *ptr = gAutomapEntry.data;

	// FIXME: This loop is implemented incorrectly. Automap requires 400x400 px,
	// but it's top offset is 105, which gives max y 505. It only works because
	// lower portions of automap data contains zeroes. If it doesn't this loop
	// will try to set pixels outside of window buffer, which usually leads to
	// crash.
	for (int y = 0; y < HEX_GRID_HEIGHT; y++) {
		for (int x = 0; x < HEX_GRID_WIDTH; x++) {
			v1 -= 1;
			if (v1 <= 0) {
				v1 = 4;
				v2 = *ptr++;
			}

			switch ((v2 & 0xC0) >> 6) {
			case 1:
				*windowBuffer++ = wallColor;
				*windowBuffer++ = wallColor;
				break;
			case 2:
				*windowBuffer++ = sceneryColor;
				*windowBuffer++ = sceneryColor;
				break;
			default:
				windowBuffer += 2;
				break;
			}

			v2 <<= 2;
		}

		windowBuffer += 640 + 240;
	}

	internal_free(gAutomapEntry.data);

	return 0;
}

// automap_pip_save
// 0x41C0F0
int automapSaveCurrent() {
	int map = mapGetCurrentMap();
	int elevation = gElevation;

	int entryOffset = gAutomapHeader.offsets[map][elevation];
	if (entryOffset < 0) {
		return 0;
	}

	debugPrint("\nAUTOMAP: Saving AutoMap DB index %d, level %d\n", map, elevation);

	bool dataBuffersAllocated = false;
	gAutomapEntry.data = (unsigned char *)internal_malloc(11024);
	if (gAutomapEntry.data != nullptr) {
		gAutomapEntry.compressedData = (unsigned char *)internal_malloc(11024);
		if (gAutomapEntry.compressedData != nullptr) {
			dataBuffersAllocated = true;
		}
	}

	if (!dataBuffersAllocated) {
		// FIXME: Leaking gAutomapEntry.data.
		debugPrint("\nAUTOMAP: Error allocating data buffers!\n");
		return -1;
	}

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();
	Common::Path automapPath(g_engine->getTargetName());

	automapPath.appendInPlace("_");
	automapPath.appendInPlace(AUTOMAP_DB);

	//	File *stream1 = fileOpen(path, "r+b");
	Common::InSaveFile *stream1 = saveMan->openForLoading(automapPath.toConfig());
	if (!stream1) {
		debugPrint("\nAUTOMAP: Error opening automap database file!\n");
		internal_free(gAutomapEntry.data);
		internal_free(gAutomapEntry.compressedData);
		return -1;
	}

	if (automapLoadHeader(stream1) == -1) {
		debugPrint("\nAUTOMAP: Error reading automap database file header!\n");
		internal_free(gAutomapEntry.data);
		internal_free(gAutomapEntry.compressedData);
		delete stream1;
		return -1;
	}

	_decode_map_data(elevation);

	int compressedDataSize = graphCompress(gAutomapEntry.data, gAutomapEntry.compressedData, 10000);
	if (compressedDataSize == -1) {
		gAutomapEntry.dataSize = 10000;
		gAutomapEntry.isCompressed = 0;
	} else {
		gAutomapEntry.dataSize = compressedDataSize;
		gAutomapEntry.isCompressed = 1;
	}

	if (entryOffset != 0) {
		debug("AUTOMAP: Saving map to TMP database (entryOffset != 0)");
		Common::Path automapTmpPath(g_engine->getTargetName());
		automapTmpPath.appendInPlace("_");
		automapTmpPath.appendInPlace(AUTOMAP_TMP);
//		File *stream2 = fileOpen(path, "wb");
		Common::OutSaveFile *stream2 = (saveMan->openForSaving(automapTmpPath.toConfig(), false));
		if (stream2 == nullptr) {
			debugPrint("\nAUTOMAP: Error creating temp file!\n");
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			delete stream1;
			return -1;
		}

//		fileRewind(stream1);
		stream1->seek(0, SEEK_SET);

		if (_copy_file_data(stream1, stream2, entryOffset) == -1) {
			debugPrint("\nAUTOMAP: Error copying file data!\n");
			delete stream1;
			stream2->finalize();
			delete stream2;
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		if (automapSaveEntry(stream2) == -1) {
			delete stream1;
			stream2->finalize();
			delete stream2;
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		int nextEntryDataSize = stream1->readSint32BE();
		if(stream1->err()) {
			debugPrint("\nAUTOMAP: Error reading database #1!\n");
			delete stream1;
			stream2->finalize();
			delete stream2;
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		int automapDataSize = stream1->size();  // fileGetSize(stream1);
		if (automapDataSize == -1) {
			debugPrint("\nAUTOMAP: Error reading database #2!\n");
			delete stream1;
			stream2->finalize();
			delete stream2;
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		int nextEntryOffset = entryOffset + nextEntryDataSize + 5;
		if (automapDataSize != nextEntryOffset) {
			if (stream1->seek(nextEntryOffset, SEEK_SET) == false) {
				debugPrint("\nAUTOMAP: Error writing temp data!\n");
				delete stream1;
				stream2->finalize();
				delete stream2;
				internal_free(gAutomapEntry.data);
				internal_free(gAutomapEntry.compressedData);
				return -1;
			}

			if (_copy_file_data(stream1, stream2, automapDataSize - nextEntryOffset) == -1) {
				debugPrint("\nAUTOMAP: Error copying file data!\n");
				delete stream1;
				stream2->finalize();
				delete stream2;
				internal_free(gAutomapEntry.data);
				internal_free(gAutomapEntry.compressedData);
				return -1;
			}
		}

		int diff = gAutomapEntry.dataSize - nextEntryDataSize;
		for (int i = 0; i < AUTOMAP_MAP_COUNT; i++) {
			for (int j = 0; j < ELEVATION_COUNT; j++) {
				if (gAutomapHeader.offsets[i][j] > entryOffset) {
					gAutomapHeader.offsets[i][j] += diff;
				}
			}
		}

		gAutomapHeader.dataSize += diff;

		if (automapSaveHeader(stream2) == -1) {
			delete stream1;
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		delete stream1;
		stream2->finalize();
		delete stream2;
		internal_free(gAutomapEntry.data);
		internal_free(gAutomapEntry.compressedData);

		if (!saveMan->removeSavefile(automapPath.toConfig())) {
			debugPrint("\nAUTOMAP: Error removing database!\n");
			return -1;
		}
		if (!saveMan->renameSavefile(automapTmpPath.toConfig(), automapPath.toConfig(), false)) {
			debugPrint("\nAUTOMAP: Error renaming database!\n");
			return -1;
		}

	} else {
		bool proceed = true;
		if (stream1->seek(0, SEEK_END)) {
			if (stream1->pos() != gAutomapHeader.dataSize) {
				warning("AUTOMAP stream1 endpos %lld differs from datasize (%d)", stream1->pos(), gAutomapHeader.dataSize);
				proceed = false;
			}
		} else {
			proceed = false;
		}

		if (!proceed) {
			debugPrint("\nAUTOMAP: Error reading automap database file header!\n");
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			delete stream1;
			return -1;
		}

		// Reopen for output
		delete stream1;
		Common::OutSaveFile *stream3 = (saveMan->openForSaving(automapPath.toConfig(), false));
		stream3->seek(gAutomapHeader.dataSize, SEEK_SET);

		if (automapSaveEntry(stream3) == -1) {
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		gAutomapHeader.offsets[map][elevation] = gAutomapHeader.dataSize;
		gAutomapHeader.dataSize += gAutomapEntry.dataSize + 5;

		if (automapSaveHeader(stream3) == -1) {
			internal_free(gAutomapEntry.data);
			internal_free(gAutomapEntry.compressedData);
			return -1;
		}

		stream3->finalize();
		if (stream3->err())
			warning("AUTOMAP: Couldn't save database");
		delete stream3;
		internal_free(gAutomapEntry.data);
		internal_free(gAutomapEntry.compressedData);
	}

	return 1;
}

// Saves automap entry into stream.
//
// 0x41C844
static int automapSaveEntry(Common::WriteStream *stream) {
	unsigned char *buffer;
	if (gAutomapEntry.isCompressed == 1) {
		buffer = gAutomapEntry.compressedData;
	} else {
		buffer = gAutomapEntry.data;
	}

	debug(5, "automapSaveEntry: Saving automap");
	stream->writeSint32BE(gAutomapEntry.dataSize);
	if (stream->err())
		goto err;
	stream->writeByte(gAutomapEntry.isCompressed);
	if (stream->err())
		goto err;
	for (int i = 0; i < gAutomapEntry.dataSize; i++) {
		stream->writeByte(*(buffer + i));
		debug(8, "automapSaveEntry: i = %d byte %u", i, *(buffer + i));
	}

	stream->flush();
	if (stream->err())
		goto err;

	return 0;

err:

	debugPrint("\nAUTOMAP: Error writing automap database entry data!\n");
	stream->finalize();
	delete stream;

	return -1;
}

// 0x41C8CC
static int automapLoadEntry(int map, int elevation) {
	gAutomapEntry.compressedData = nullptr;

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();
	Common::Path automapPath(g_engine->getTargetName());

	automapPath.appendInPlace("_");
	automapPath.appendInPlace(AUTOMAP_DB);

	bool success = true;

//	File *stream = fileOpen(path, "r+b");
	Common::InSaveFile *stream = saveMan->openForLoading(automapPath.toConfig());
	if (stream == nullptr) {
		debugPrint("\nAUTOMAP: Error opening automap database file!\n");
//		debugPrint("Error continued: AM_ReadEntry: path: %s", path);
		return -1;
	}

	if (automapLoadHeader(stream) == -1) {
		debugPrint("\nAUTOMAP: Error reading automap database header!\n");
		delete stream;
		return -1;
	}

	if (gAutomapHeader.offsets[map][elevation] <= 0) {
		success = false;
		goto out;
	}

	if(stream->seek(gAutomapHeader.offsets[map][elevation], SEEK_SET) != true) {
		success = false;
		goto out;
	}

	gAutomapEntry.dataSize = stream->readSint32BE();
	if(stream->err()) {
		success = false;
		goto out;
	}
	// if (_db_freadInt(stream, &(gAutomapEntry.dataSize)) == -1) {
	// 	success = false;
	// 	goto out;
	// }

	gAutomapEntry.isCompressed = stream->readByte();
	if (stream->err()) {
		success = false;
		goto out;
	}

	// if (fileReadUInt8(stream, &(gAutomapEntry.isCompressed)) == -1) {
	// 	success = false;
	// 	goto out;
	// }

	if (gAutomapEntry.isCompressed == 1) {
		gAutomapEntry.compressedData = (unsigned char *)internal_malloc(11024);
		if (gAutomapEntry.compressedData == nullptr) {
			debugPrint("\nAUTOMAP: Error allocating decompression buffer!\n");
			delete stream;
			return -1;
		}

		for (int i = 0; i < gAutomapEntry.dataSize; i++)
			*(gAutomapEntry.compressedData + i) = stream->readByte();
		if (stream->err()) {
			success = false;
			goto out;
		}
//		if (fileReadUInt8List(stream, gAutomapEntry.compressedData, gAutomapEntry.dataSize) == -1) {
//			success = 0;
//			goto out;
//		}

		if (graphDecompress(gAutomapEntry.compressedData, gAutomapEntry.data, 10000) == -1) {
			debugPrint("\nAUTOMAP: Error decompressing DB entry!\n");
			delete stream;
			return -1;
		}
	} else {
		for (int i = 0; i < gAutomapEntry.dataSize; i++)
			*(gAutomapEntry.data + i) = stream->readByte();
		if (stream->err()) {
			success = false;
			goto out;
		}
//		if (fileReadUInt8List(stream, gAutomapEntry.data, gAutomapEntry.dataSize) == -1) {
//			success = false;
//			goto out;
//		}
	}

out:

	delete stream;

	if (!success) {
		debugPrint("\nAUTOMAP: Error reading automap database entry data!\n");

		return -1;
	}

	if (gAutomapEntry.compressedData != nullptr) {
		internal_free(gAutomapEntry.compressedData);
	}

	return 0;
}

// Saves automap.db header.
//
// 0x41CAD8
static int automapSaveHeader(Common::SeekableWriteStream *stream) {
	// fileRewind(stream);
	stream->seek(0, SEEK_SET);

	stream->writeByte(gAutomapHeader.version);
	stream->writeSint32BE(gAutomapHeader.dataSize);

	int *arr = (int *) gAutomapHeader.offsets;
	for (int i = 0; i < AUTOMAP_OFFSET_COUNT; ++i) {
		int value = arr[i];
		stream->writeSint32BE(value);
	}

	if (stream->err())
		goto err;

/*	if (fileWriteUInt8(stream, gAutomapHeader.version) == -1) {
		goto err;
	}

	if (_db_fwriteLong(stream, gAutomapHeader.dataSize) == -1) {
		goto err;
	}

	if (_db_fwriteLongCount(stream, (int *)gAutomapHeader.offsets, AUTOMAP_OFFSET_COUNT) == -1) {
		goto err;
	}*/

	return 0;

err:

	debugPrint("\nAUTOMAP: Error writing automap database header!\n");

	stream->finalize();
	delete(stream);

	return -1;
}

// Loads automap.db header.
//
// 0x41CB50
static int automapLoadHeader(Common::ReadStream *stream) {

	gAutomapHeader.version = stream->readByte();
//	if (fileReadUInt8(stream, &(gAutomapHeader.version)) == -1) {
//		return -1;
//	}

	gAutomapHeader.dataSize = stream->readSint32BE();
//	if (_db_freadInt(stream, &(gAutomapHeader.dataSize)) == -1) {
//		return -1;
//	}

	int *arr = (int *) gAutomapHeader.offsets;
	for (int i = 0; i < AUTOMAP_OFFSET_COUNT; ++i)
		arr[i] = stream->readSint32BE();
//	if (_db_freadIntCount(stream, (int *)gAutomapHeader.offsets, AUTOMAP_OFFSET_COUNT) == -1) {
//		return -1;
//	}

	if (gAutomapHeader.version != 1) {
		return -1;
	}

	debug(5, "AUTOMAP header loaded succesfully");
	return 0;
}

// 0x41CBA4
static void _decode_map_data(int elevation) {
	memset(gAutomapEntry.data, 0, SQUARE_GRID_SIZE);

	_obj_process_seen();

	Object *object = objectFindFirstAtElevation(elevation);
	while (object != nullptr) {
		if (object->tile != -1 && (object->flags & OBJECT_SEEN) != 0) {
			int contentType;

			int objectType = FID_TYPE(object->fid);
			if (objectType == OBJ_TYPE_SCENERY && object->pid != PROTO_ID_0x2000158) {
				contentType = 2;
			} else if (objectType == OBJ_TYPE_WALL) {
				contentType = 1;
			} else {
				contentType = 0;
			}

			if (contentType != 0) {
				int v1 = 200 - object->tile % 200;
				int v2 = v1 / 4 + 50 * (object->tile / 200);
				int v3 = 2 * (3 - v1 % 4);
				gAutomapEntry.data[v2] &= ~(0x03 << v3);
				gAutomapEntry.data[v2] |= (contentType << v3);
			}
		}
		object = objectFindNextAtElevation();
	}
}

// 0x41CC98
static int automapCreate() {
	gAutomapHeader.version = 1;
	gAutomapHeader.dataSize = 1925;
	memcpy(gAutomapHeader.offsets, _defam, sizeof(_defam));

	Common::SaveFileManager *saveMan = g_system->getSavefileManager();
	Common::Path automapPath(g_engine->getTargetName());

	automapPath.appendInPlace("_");
	automapPath.appendInPlace(AUTOMAP_DB);
//	File *stream = fileOpen(path, "wb");
	Common::OutSaveFile *stream = (saveMan->openForSaving(automapPath.toConfig(), false));
	if (!stream) {
		debugPrint("\nAUTOMAP: Error creating automap database file!\n");
		return -1;
	}

	if (automapSaveHeader(stream) == -1) {
		return -1;
	}

	stream->finalize();
	delete stream;

	return 0;
}

// Copy data from stream1 to stream2.
//
// 0x41CD6C
static int _copy_file_data(Common::ReadStream *stream1, Common::WriteStream *stream2, int length) {
	void *buffer = internal_malloc(0xFFFF);
	if (buffer == nullptr) {
		return -1;
	}

	// NOTE: Original code is slightly different, but does the same thing.
	while (length != 0) {
		int chunkLength = MIN(length, 0xFFFF);

		if (stream1->read(buffer, chunkLength) < (uint)chunkLength)
			break;
		if (stream2->write(buffer, chunkLength) < (uint)chunkLength)
			break;

		length -= chunkLength;
	}

	internal_free(buffer);

	if (length != 0) {
		warning("AUTOMAP copyfile: remaining length is nonzero: %d", length);
		return -1;
	}

	return 0;
}

// 0x41CE74
int automapGetHeader(AutomapHeader **automapHeaderPtr) {

	// File *stream = fileOpen(path, "rb");
	Common::SaveFileManager *saveMan = g_system->getSavefileManager();
	Common::Path automapPath(g_engine->getTargetName());

	automapPath.appendInPlace("_");
	automapPath.appendInPlace(AUTOMAP_DB);
	Common::InSaveFile *stream = saveMan->openForLoading(automapPath.toConfig());

	if (stream == nullptr) {
		debugPrint("\nAUTOMAP: Error opening database file for reading!\n");
		return -1;
	}

	if (automapLoadHeader(stream) == -1) {
		debugPrint("\nAUTOMAP: Error reading automap database header pt2!\n");
		delete stream;
		return -1;
	}

	delete stream;

	*automapHeaderPtr = &gAutomapHeader;

	return 0;
}

void automapSetDisplayMap(int map, bool available) {
	if (map >= 0 && map < AUTOMAP_MAP_COUNT) {
		_displayMapList[map] = available ? 0 : -1;
	}
}

} // namespace Fallout2
