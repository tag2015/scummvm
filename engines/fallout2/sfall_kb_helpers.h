#ifndef FALLOUT2_SFALL_KB_HELPERS_H
#define FALLOUT2_SFALL_KB_HELPERS_H

namespace Fallout2 {

/// Returns `true` if given key is pressed.
bool sfall_kb_is_key_pressed(int key);

/// Simulates pressing `key`.
void sfall_kb_press_key(int key);

} // namespace Fallout2

#endif
