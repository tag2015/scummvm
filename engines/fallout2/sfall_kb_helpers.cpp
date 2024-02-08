#include "fallout2/sfall_kb_helpers.h"

// #include <SDL.h>

#include "fallout2/fallout2.h"

#include "common/events.h"
#include "common/keyboard.h"

namespace Fallout2 {

/// Maps DirectInput DIK constants to SDL scancodes.
static Common::KeyCode kDiks[256] = {
	Common::KEYCODE_INVALID,
	Common::KEYCODE_ESCAPE,       // DIK_ESCAPE
	Common::KEYCODE_1,            // DIK_1
	Common::KEYCODE_2,            // DIK_2
	Common::KEYCODE_3,            // DIK_3
	Common::KEYCODE_4,            // DIK_4
	Common::KEYCODE_5,            // DIK_5
	Common::KEYCODE_6,            // DIK_6
	Common::KEYCODE_7,            // DIK_7
	Common::KEYCODE_8,            // DIK_8
	Common::KEYCODE_9,            // DIK_9
	Common::KEYCODE_0,            // DIK_0
	Common::KEYCODE_MINUS,        // DIK_MINUS
	Common::KEYCODE_EQUALS,       // DIK_EQUALS
	Common::KEYCODE_BACKSPACE,    // DIK_BACK
	Common::KEYCODE_TAB,          // DIK_TAB
	Common::KEYCODE_q,            // DIK_Q
	Common::KEYCODE_w,            // DIK_W
	Common::KEYCODE_e,            // DIK_E
	Common::KEYCODE_r,            // DIK_R
	Common::KEYCODE_t,            // DIK_T
	Common::KEYCODE_y,            // DIK_Y
	Common::KEYCODE_u,            // DIK_U
	Common::KEYCODE_i,            // DIK_I
	Common::KEYCODE_o,            // DIK_O
	Common::KEYCODE_p,            // DIK_P
	Common::KEYCODE_LEFTBRACKET,  // DIK_LBRACKET
	Common::KEYCODE_RIGHTBRACKET, // DIK_RBRACKET
	Common::KEYCODE_RETURN,       // DIK_RETURN
	Common::KEYCODE_LCTRL,        // DIK_LCONTROL
	Common::KEYCODE_a,            // DIK_A
	Common::KEYCODE_s,            // DIK_S
	Common::KEYCODE_d,            // DIK_D
	Common::KEYCODE_f,            // DIK_F
	Common::KEYCODE_g,            // DIK_G
	Common::KEYCODE_h,            // DIK_H
	Common::KEYCODE_j,            // DIK_J
	Common::KEYCODE_k,            // DIK_K
	Common::KEYCODE_l,            // DIK_L
	Common::KEYCODE_SEMICOLON,    // DIK_SEMICOLON
	Common::KEYCODE_QUOTE,   	  // DIK_APOSTROPHE
	Common::KEYCODE_BACKQUOTE,    // DIK_GRAVE
	Common::KEYCODE_LSHIFT,       // DIK_LSHIFT
	Common::KEYCODE_BACKSLASH,    // DIK_BACKSLASH
	Common::KEYCODE_z,            // DIK_Z
	Common::KEYCODE_x,            // DIK_X
	Common::KEYCODE_c,            // DIK_C
	Common::KEYCODE_v,            // DIK_V
	Common::KEYCODE_b,            // DIK_B
	Common::KEYCODE_n,            // DIK_N
	Common::KEYCODE_m,            // DIK_M
	Common::KEYCODE_COMMA,        // DIK_COMMA
	Common::KEYCODE_PERIOD,       // DIK_PERIOD
	Common::KEYCODE_SLASH,        // DIK_SLASH
	Common::KEYCODE_RSHIFT,       // DIK_RSHIFT
	Common::KEYCODE_KP_MULTIPLY,  // DIK_MULTIPLY
	Common::KEYCODE_LALT,         // DIK_LMENU
	Common::KEYCODE_SPACE,        // DIK_SPACE
	Common::KEYCODE_CAPSLOCK,     // DIK_CAPITAL
	Common::KEYCODE_F1,           // DIK_F1
	Common::KEYCODE_F2,           // DIK_F2
	Common::KEYCODE_F3,           // DIK_F3
	Common::KEYCODE_F4,           // DIK_F4
	Common::KEYCODE_F5,           // DIK_F5
	Common::KEYCODE_F6,           // DIK_F6
	Common::KEYCODE_F7,           // DIK_F7
	Common::KEYCODE_F8,           // DIK_F8
	Common::KEYCODE_F9,           // DIK_F9
	Common::KEYCODE_F10,          // DIK_F10
	Common::KEYCODE_NUMLOCK,	  // DIK_NUMLOCK
	Common::KEYCODE_SCROLLOCK,    // DIK_SCROLL
	Common::KEYCODE_KP7,          // DIK_NUMPAD7
	Common::KEYCODE_KP8,          // DIK_NUMPAD8
	Common::KEYCODE_KP9,          // DIK_NUMPAD9
	Common::KEYCODE_KP_MINUS,     // DIK_SUBTRACT
	Common::KEYCODE_KP4,          // DIK_NUMPAD4
	Common::KEYCODE_KP5,          // DIK_NUMPAD5
	Common::KEYCODE_KP6,          // DIK_NUMPAD6
	Common::KEYCODE_KP_PLUS,      // DIK_ADD
	Common::KEYCODE_KP1,          // DIK_NUMPAD1
	Common::KEYCODE_KP2,          // DIK_NUMPAD2
	Common::KEYCODE_KP3,          // DIK_NUMPAD3
	Common::KEYCODE_KP0,          // DIK_NUMPAD0
	Common::KEYCODE_KP_PERIOD,    // DIK_DECIMAL
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_F11, 		  // DIK_F11
	Common::KEYCODE_F12, 		  // DIK_F12
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_KP_EQUALS,  // DIK_NUMPADEQUALS
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID, 	// DIK_AT
	Common::KEYCODE_INVALID, 	// DIK_COLON
	Common::KEYCODE_INVALID, 	// DIK_UNDERLINE
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID, 	// DIK_STOP
	Common::KEYCODE_INVALID, 	// DIK_AX
	Common::KEYCODE_INVALID, 	// DIK_UNLABELED
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_KP_ENTER,	// DIK_NUMPADENTER
	Common::KEYCODE_RCTRL,      // DIK_RCONTROL
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_KP_PERIOD,  // DIK_NUMPADCOMMA
	Common::KEYCODE_INVALID,
	Common::KEYCODE_SLASH,  	// DIK_DIVIDE
	Common::KEYCODE_INVALID,
	Common::KEYCODE_SYSREQ, 	// DIK_SYSRQ
	Common::KEYCODE_RALT,  		// DIK_RMENU
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_HOME,     // DIK_HOME
	Common::KEYCODE_UP,       // DIK_UP
	Common::KEYCODE_PAGEUP,   // DIK_PRIOR
	Common::KEYCODE_INVALID,
	Common::KEYCODE_LEFT,     // DIK_LEFT
	Common::KEYCODE_INVALID,
	Common::KEYCODE_RIGHT,    // DIK_RIGHT
	Common::KEYCODE_INVALID,
	Common::KEYCODE_END,      // DIK_END
	Common::KEYCODE_DOWN,     // DIK_DOWN
	Common::KEYCODE_PAGEDOWN, // DIK_NEXT
	Common::KEYCODE_INSERT,   // DIK_INSERT
	Common::KEYCODE_DELETE,   // DIK_DELETE
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_LSUPER,   // DIK_LWIN
	Common::KEYCODE_RSUPER,   // DIK_RWIN
	Common::KEYCODE_COMPOSE,  // DIK_APPS
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
	Common::KEYCODE_INVALID,
};

/// Translates Sfall key code (DIK or VK constant) to SDL scancode.
static Common::KeyCode get_scancode_from_key(int key) {
	return kDiks[key & 0xFF];
}

bool sfall_kb_is_key_pressed(int key) {
	Common::KeyCode scancode = get_scancode_from_key(key);
	if (scancode == Common::KEYCODE_INVALID) {
		return false;
	}

	// TODO unsure if this works
	Common::Event e;
	while (g_system->getEventManager()->pollEvent(e)) {
		if (e.type == Common::EVENT_KEYDOWN) {
			return (e.kbd.keycode == scancode);
		}
	}
	return false;
}

void sfall_kb_press_key(int key) {
	Common::KeyCode scancode = get_scancode_from_key(key);
	if (scancode == Common::KEYCODE_INVALID) {
		return;
	}

	Common::Event event;
	event.kbd.keycode = scancode;

	event.type = Common::EVENT_KEYDOWN;
	g_system->getEventManager()->pushEvent(event);

	event.type = Common::EVENT_KEYUP;
	g_system->getEventManager()->pushEvent(event);
}

} // namespace Fallout2
