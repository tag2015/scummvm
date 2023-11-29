#include "fallout2/fallout2.h"

#include "fallout2/dinput.h"
#include "fallout2/mouse.h"
#include "fallout2/svga.h"

namespace Fallout2 {

static int gMouseWheelDeltaX = 0;
static int gMouseWheelDeltaY = 0;

static int lastMousePosX = -1;
static int lastMousePosY = -1;

// 0x4E0400
bool directInputInit() {
	//	if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
	//		return false;
	//	}

	if (!mouseDeviceInit()) {
		goto err;
	}

	if (!keyboardDeviceInit()) {
		goto err;
	}

	return true;

err:

	directInputFree();

	return false;
}

// 0x4E0478
void directInputFree() {
	//	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

// 0x4E04E8
bool mouseDeviceAcquire() {
	return true;
}

// 0x4E0514
bool mouseDeviceUnacquire() {
	return true;
}

// 0x4E053C
bool mouseDeviceGetData(MouseData *mouseState) {

	// CE: This function is sometimes called outside loops calling `get_input`
	// and subsequently `GNW95_process_message`, so mouse events might not be
	// handled by SDL yet.
	//
	// TODO: Move mouse events processing into `GNW95_process_message` and
	// update mouse position manually.

	// SDL_PumpEvents();

	Common::Point mousePos = g_engine->getEventManager()->getMousePos();
	// EventManager can't return the relative mouse position, so keep track manually
	if (lastMousePosX == -1) { // first time
		g_system->warpMouse(_scr_size.right / 2, _scr_size.bottom / 2);
		_mouse_set_position(-1, -1); // force clip
		mousePos = g_engine->getEventManager()->getMousePos();
		mouseState->x = mousePos.x;
		mouseState->y = mousePos.y;
	} else {
		mouseState->x = mousePos.x - lastMousePosX;
		mouseState->y = mousePos.y - lastMousePosY;
	}
	lastMousePosX = mousePos.x;
	lastMousePosY = mousePos.y;
	mouseState->buttons[0] = g_engine->getEventManager()->getButtonState() & 1;
	mouseState->buttons[1] = (g_engine->getEventManager()->getButtonState() & 2) >> 1;

	mouseState->wheelX = gMouseWheelDeltaX;
	mouseState->wheelY = gMouseWheelDeltaY;

	gMouseWheelDeltaX = 0;
	gMouseWheelDeltaY = 0;

	return true;
}

// 0x4E05A8
bool keyboardDeviceAcquire() {
	return true;
}

// 0x4E05D4
bool keyboardDeviceUnacquire() {
	return true;
}

// 0x4E05FC
bool keyboardDeviceReset() {
	//	SDL_FlushEvents(SDL_KEYDOWN, SDL_TEXTINPUT);
	return true;
}

// 0x4E0650
bool keyboardDeviceGetData(KeyboardData *keyboardData) {
	return true;
}

// 0x4E070C
bool mouseDeviceInit() {
	// return SDL_SetRelativeMouseMode(SDL_TRUE) == 0;
	return true;
}

// 0x4E078C
void mouseDeviceFree() {
}

// 0x4E07B8
bool keyboardDeviceInit() {
	return true;
}

// 0x4E0874
void keyboardDeviceFree() {
}

void handleMouseEvent(const Common::Event *event) {
	// Mouse movement and buttons are accumulated in SDL itself and will be
	// processed later in `mouseDeviceGetData` via `SDL_GetRelativeMouseState`.

	if (event->type == Common::EVENT_WHEELUP)
		gMouseWheelDeltaY += 5;
	else if (event->type == Common::EVENT_WHEELDOWN) {
		gMouseWheelDeltaY -= 5;
	}
}

} // namespace Fallout2
