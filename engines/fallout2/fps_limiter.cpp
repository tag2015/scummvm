#include "fallout2/fps_limiter.h"

#include "fallout2/fallout2.h"

// #include <SDL.h>

namespace Fallout2 {

FpsLimiter::FpsLimiter(unsigned int fps)
	: _fps(fps), _ticks(0) {
}

void FpsLimiter::mark() {
	_ticks = g_system->getMillis();
}

void FpsLimiter::throttle() const {
	if (1000 / _fps > g_system->getMillis() - _ticks) {
		g_system->delayMillis(1000 / _fps - (g_system->getMillis() - _ticks));
	}
}

} // namespace Fallout2
