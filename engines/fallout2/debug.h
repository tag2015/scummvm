#ifndef FALLOUT2_DEBUG_H
#define FALLOUT2_DEBUG_H

#include "common/debug.h"

namespace Fallout2 {

template<typename... Args>
void debugPrint(const char *format, Args... args) {
	debug(1, format, args...);
}

} // namespace Fallout2

#endif
