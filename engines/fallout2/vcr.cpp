#include "fallout2/vcr.h"

// #include <stdlib.h>

#include "fallout2/fallout2.h"

#include "fallout2/delay.h"
#include "fallout2/input.h"
#include "fallout2/kb.h"
#include "fallout2/memory.h"
#include "fallout2/mouse.h"
#include "fallout2/svga.h"

namespace Fallout2 {

static bool vcrInitBuffer();
static bool vcrFreeBuffer();
static bool vcrClear();
static bool vcrLoad();

// 0x51E2F0
VcrEntry *_vcr_buffer = nullptr;

// number of entries in _vcr_buffer
// 0x51E2F4
int _vcr_buffer_index = 0;

// 0x51E2F8
unsigned int gVcrState = VCR_STATE_TURNED_OFF;

// 0x51E2FC
unsigned int _vcr_time = 0;

// 0x51E300
unsigned int _vcr_counter = 0;

// 0x51E304
unsigned int gVcrTerminateFlags = 0;

// 0x51E308
int gVcrPlaybackCompletionReason = VCR_PLAYBACK_COMPLETION_REASON_NONE;

// 0x51E30C
static unsigned int _vcr_start_time = 0;

// 0x51E310
static int _vcr_registered_atexit = 0;

// 0x51E314
static Common::InSaveFile *gVcrFileIn = nullptr;
static Common::OutSaveFile *gVcrFileOut = nullptr;

// 0x51E318
static int _vcr_buffer_end = 0;

// 0x51E31C
static VcrPlaybackCompletionCallback *gVcrPlaybackCompletionCallback = nullptr;

// 0x51E320
static unsigned int gVcrRequestedTerminationFlags = 0;

// 0x51E324
static int gVcrOldKeyboardLayout = 0;

// 0x6AD940
static VcrEntry stru_6AD940;

// 0x4D2680
bool vcrRecord(const char *fileName) {
	if (gVcrState != VCR_STATE_TURNED_OFF) {
		return false;
	}

	if (fileName == nullptr) {
		return false;
	}

	// NOTE: Uninline.
	if (!vcrInitBuffer()) {
		return false;
	}

	Common::String vcrPath(g_engine->getTargetName() + "_" + fileName);
	vcrPath.replace('\\', '_');

	gVcrFileOut = g_system->getSavefileManager()->openForSaving(Common::String(vcrPath));
	if (gVcrFileOut == nullptr) {
		// NOTE: Uninline.
		vcrFreeBuffer();
		return false;
	}

	if (_vcr_registered_atexit == 0) {
		//  _vcr_registered_atexit = atexit(vcrStop);
	}

	VcrEntry *vcrEntry = &(_vcr_buffer[_vcr_buffer_index]);
	vcrEntry->type = VCR_ENTRY_TYPE_INITIAL_STATE;
	vcrEntry->time = 0;
	vcrEntry->counter = 0;
	vcrEntry->initial.keyboardLayout = keyboardGetLayout();

	while (mouseGetEvent() != 0) {
		_mouse_info();
	}

	mouseGetPosition(&(vcrEntry->initial.mouseX), &(vcrEntry->initial.mouseY));

	_vcr_counter = 1;
	_vcr_buffer_index++;
	_vcr_start_time = getTicks();
	keyboardReset();
	gVcrState = VCR_STATE_RECORDING;

	debug(1, "Recording to VCR file %s", vcrPath.c_str());
	return true;
}

// 0x4D27EC
bool vcrPlay(const char *fileName, unsigned int terminationFlags, VcrPlaybackCompletionCallback *callback) {
	if (gVcrState != VCR_STATE_TURNED_OFF) {
		return false;
	}

	if (fileName == nullptr) {
		return false;
	}

	// NOTE: Uninline.
	if (!vcrInitBuffer()) {
		return false;
	}

	Common::String vcrPath(g_engine->getTargetName() + "_" + fileName);
	vcrPath.replace('\\', '_');

	gVcrFileIn = g_system->getSavefileManager()->openForLoading(Common::String(vcrPath));
	if (gVcrFileIn == nullptr) {
		// NOTE: Uninline.
		vcrFreeBuffer();
		return false;
	}

	if (!vcrLoad()) {
		delete gVcrFileIn;
		// NOTE: Uninline.
		vcrFreeBuffer();
		return false;
	}

	while (mouseGetEvent() != 0) {
		_mouse_info();
	}

	keyboardReset();

	gVcrRequestedTerminationFlags = terminationFlags;
	gVcrPlaybackCompletionCallback = callback;
	gVcrPlaybackCompletionReason = VCR_PLAYBACK_COMPLETION_REASON_COMPLETED;
	gVcrTerminateFlags = 0;
	_vcr_counter = 0;
	_vcr_time = 0;
	_vcr_start_time = getTicks();
	gVcrState = VCR_STATE_PLAYING;
	stru_6AD940.time = 0;
	stru_6AD940.counter = 0;

	debug(1, "Playing VCR file %s", vcrPath.c_str());
	return true;
}

// 0x4D28F4
void vcrStop() {
	if (gVcrState == VCR_STATE_RECORDING || gVcrState == VCR_STATE_PLAYING) {
		gVcrState |= VCR_STATE_STOP_REQUESTED;
	}

	keyboardReset();
}

// 0x4D2918
int vcrGetState() {
	return gVcrState;
}

// 0x4D2930
int vcrUpdate() {
	if ((gVcrState & VCR_STATE_STOP_REQUESTED) != 0) {
		gVcrState &= ~VCR_STATE_STOP_REQUESTED;

		switch (gVcrState) {
		case VCR_STATE_RECORDING:
			vcrDump();
			gVcrFileOut->finalize();
			delete gVcrFileOut;
			gVcrFileOut = nullptr;

			// NOTE: Uninline.
			vcrFreeBuffer();

			break;
		case VCR_STATE_PLAYING:
			delete gVcrFileIn;
			gVcrFileIn = nullptr;

			// NOTE: Uninline.
			vcrFreeBuffer();

			keyboardSetLayout(gVcrOldKeyboardLayout);

			if (gVcrPlaybackCompletionCallback != nullptr) {
				gVcrPlaybackCompletionCallback(gVcrPlaybackCompletionReason);
			}
			break;
		}

		gVcrState = VCR_STATE_TURNED_OFF;
	}

	switch (gVcrState) {
	case VCR_STATE_RECORDING:
		_vcr_counter++;
		_vcr_time = getTicksSince(_vcr_start_time);
		if (_vcr_buffer_index == VCR_BUFFER_CAPACITY - 1) {
			vcrDump();
		}
		break;
	case VCR_STATE_PLAYING:
		if (_vcr_buffer_index < _vcr_buffer_end || vcrLoad()) {
			VcrEntry *vcrEntry = &(_vcr_buffer[_vcr_buffer_index]);
			if (stru_6AD940.counter < vcrEntry->counter) {
				if (vcrEntry->time > stru_6AD940.time) {
					unsigned int delay = stru_6AD940.time;
					delay += (_vcr_counter - stru_6AD940.counter) * (vcrEntry->time - stru_6AD940.time) / (vcrEntry->counter - stru_6AD940.counter);

					delay_ms(delay - (getTicks() - _vcr_start_time));
				}
			}

			_vcr_counter++;

			int rc = 0;
			while (_vcr_counter >= _vcr_buffer[_vcr_buffer_index].counter) {
				_vcr_time = getTicksSince(_vcr_start_time);
				if (_vcr_time > _vcr_buffer[_vcr_buffer_index].time + 5 || _vcr_time < _vcr_buffer[_vcr_buffer_index].time - 5) {
					_vcr_start_time += _vcr_time - _vcr_buffer[_vcr_buffer_index].time;
				}

				switch (_vcr_buffer[_vcr_buffer_index].type) {
				case VCR_ENTRY_TYPE_INITIAL_STATE:
					gVcrState = VCR_STATE_TURNED_OFF;
					gVcrOldKeyboardLayout = keyboardGetLayout();
					keyboardSetLayout(_vcr_buffer[_vcr_buffer_index].initial.keyboardLayout);
					while (mouseGetEvent() != 0) {
						_mouse_info();
					}
					gVcrState = VCR_ENTRY_TYPE_INITIAL_STATE;
					mouseHideCursor();
					_mouse_set_position(_vcr_buffer[_vcr_buffer_index].initial.mouseX, _vcr_buffer[_vcr_buffer_index].initial.mouseY);
					mouseShowCursor();
					keyboardReset();
					gVcrTerminateFlags = gVcrRequestedTerminationFlags;
					_vcr_start_time = getTicks();
					_vcr_counter = 0;
					break;
				case VCR_ENTRY_TYPE_KEYBOARD_EVENT:
					if (1) {
						KeyboardData keyboardData;
						keyboardData.key = _vcr_buffer[_vcr_buffer_index].keyboardEvent.key;
						_kb_simulate_key(&keyboardData);
					}
					break;
				case VCR_ENTRY_TYPE_MOUSE_EVENT:
					rc = 3;
					_mouse_simulate_input(_vcr_buffer[_vcr_buffer_index].mouseEvent.dx, _vcr_buffer[_vcr_buffer_index].mouseEvent.dy, _vcr_buffer[_vcr_buffer_index].mouseEvent.buttons);
					break;
				}

				memcpy(&stru_6AD940, &(_vcr_buffer[_vcr_buffer_index]), sizeof(stru_6AD940));
				_vcr_buffer_index++;
			}

			return rc;
		} else {
			// NOTE: Uninline.
			vcrStop();
		}
		break;
	}

	return 0;
}

// NOTE: Inlined.
//
// 0x4D2C64
static bool vcrInitBuffer() {
	if (_vcr_buffer == nullptr) {
		_vcr_buffer = (VcrEntry *)internal_malloc(sizeof(*_vcr_buffer) * VCR_BUFFER_CAPACITY);
		if (_vcr_buffer == nullptr) {
			return false;
		}
	}

	// NOTE: Uninline.
	vcrClear();

	return true;
}

// NOTE: Inlined.
//
// 0x4D2C98
static bool vcrFreeBuffer() {
	if (_vcr_buffer == nullptr) {
		return false;
	}

	// NOTE: Uninline.
	vcrClear();

	internal_free(_vcr_buffer);
	_vcr_buffer = nullptr;

	return true;
}

// 0x4D2CD0
static bool vcrClear() {
	if (_vcr_buffer == nullptr) {
		return false;
	}

	_vcr_buffer_index = 0;

	return true;
}

// 0x4D2CF0
bool vcrDump() {
	if (_vcr_buffer == nullptr) {
		return false;
	}

	if (gVcrFileOut == nullptr) {
		return false;
	}

	for (int index = 0; index < _vcr_buffer_index; index++) {
		if (!vcrWriteEntry(&(_vcr_buffer[index]), gVcrFileOut)) {
			return false;
		}
	}

	// NOTE: Uninline.
	if (!vcrClear()) {
		return false;
	}

	return true;
}

// 0x4D2D74
static bool vcrLoad() {
	if (gVcrFileIn == nullptr) {
		return false;
	}

	// NOTE: Uninline.
	if (!vcrClear()) {
		return false;
	}

	for (_vcr_buffer_end = 0; _vcr_buffer_end < VCR_BUFFER_CAPACITY; _vcr_buffer_end++) {
		if (!vcrReadEntry(&(_vcr_buffer[_vcr_buffer_end]), gVcrFileIn)) {
			break;
		}
	}

	if (_vcr_buffer_end == 0) {
		return false;
	}

	return true;
}

// 0x4D2E00
bool vcrWriteEntry(VcrEntry *vcrEntry, Common::OutSaveFile *stream) {
	stream->writeUint32BE(vcrEntry->type);
	stream->writeUint32BE(vcrEntry->time);
	stream->writeUint32BE(vcrEntry->counter);
	if (stream->err())
		return false;

	switch (vcrEntry->type) {
	case VCR_ENTRY_TYPE_INITIAL_STATE:
		stream->writeSint32BE(vcrEntry->initial.mouseX);
		stream->writeSint32BE(vcrEntry->initial.mouseY);
		stream->writeSint32BE(vcrEntry->initial.keyboardLayout);
		break;

	case VCR_ENTRY_TYPE_KEYBOARD_EVENT:
		stream->writeUint16BE(vcrEntry->keyboardEvent.key);
		break;

	case VCR_ENTRY_TYPE_MOUSE_EVENT:
		stream->writeSint32BE(vcrEntry->mouseEvent.dx);
		stream->writeSint32BE(vcrEntry->mouseEvent.dy);
		stream->writeSint32BE(vcrEntry->mouseEvent.buttons);
	}

	if (stream->err())
		return false;

	return true;
}

// 0x4D2EE4
bool vcrReadEntry(VcrEntry *vcrEntry, Common::InSaveFile *stream) {
	vcrEntry->type = stream->readUint32BE();
	vcrEntry->time = stream->readUint32BE();
	vcrEntry->counter = stream->readUint32BE();
	if (stream->err())
		return false;

	switch (vcrEntry->type) {
	case VCR_ENTRY_TYPE_INITIAL_STATE:
		vcrEntry->initial.mouseX = stream->readSint32BE();
		vcrEntry->initial.mouseY = stream->readSint32BE();
		vcrEntry->initial.keyboardLayout = stream->readSint32BE();
		break;

	case VCR_ENTRY_TYPE_KEYBOARD_EVENT:
		vcrEntry->keyboardEvent.key = stream->readUint16BE();
		break;

	case VCR_ENTRY_TYPE_MOUSE_EVENT:
		vcrEntry->mouseEvent.dx = stream->readSint32BE();
		vcrEntry->mouseEvent.dy = stream->readSint32BE();
		vcrEntry->mouseEvent.buttons = stream->readSint32BE();
	}

	if (stream->err())
		return false;

	return true;
}

} // namespace Fallout2
