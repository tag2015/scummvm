#ifndef FALLOUT2_DINPUT_H
#define FALLOUT2_DINPUT_H

// #include <SDL.h>

#include "common/events.h"

namespace Fallout2 {

typedef struct MouseData {
	int x;
	int y;
	unsigned char buttons[2];
	int wheelX;
	int wheelY;
} MouseData;

typedef struct KeyboardData {
	int key;
	int ascii;
	char down;
} KeyboardData;

bool directInputInit();
void directInputFree();
bool mouseDeviceAcquire();
bool mouseDeviceUnacquire();
bool mouseDeviceGetData(MouseData *mouseData);
bool keyboardDeviceAcquire();
bool keyboardDeviceUnacquire();
bool keyboardDeviceReset();
bool keyboardDeviceGetData(KeyboardData *keyboardData);
bool mouseDeviceInit();
void mouseDeviceFree();
bool keyboardDeviceInit();
void keyboardDeviceFree();

void handleMouseEvent(const Common::Event *event);
void handleTouchEvent(const Common::Event *event);

} // namespace Fallout2

#endif
