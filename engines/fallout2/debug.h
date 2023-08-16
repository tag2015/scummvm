#ifndef FALLOUT2_DEBUG_H
#define FALLOUT2_DEBUG_H

#include "common/debug.h"

namespace Fallout2 {

/* typedef int(DebugPrintProc)(char *string);

void _GNW_debug_init();
void _debug_register_mono();
void _debug_register_log(const char *fileName, const char *mode);
void _debug_register_screen();
void _debug_register_env();
void _debug_register_func(DebugPrintProc *proc);
int debugPrint(const char *format, ...);
void _debug_exit(void);
*/

template<typename... Args>
void debugPrint(const char *format, Args... args) {
	debug(format, args...);
}

} // namespace Fallout2

#endif /* DEBUG_H */
